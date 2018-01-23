#!/bin/bash
action=$1
VERBOSE=-v
VERBOSE=
# device=/dev/ttyUSB0
device=$2
# -------------------------------------------------------------------------
function creaLink() {
    ln -s /home/pi/485d_Loreto/Arduino/LnLibraries/LnFunctions      /home/pi/485d_Loreto/Arduino/rs485-Full/lib/LnFunctions
    ln -s /home/pi/485d_Loreto/Arduino/LnLibraries/RS485_protocol   /home/pi/485d_Loreto/Arduino/rs485-Full/lib/RS485_protocol
}

function deleteLink() {
    rm -f /home/pi/485d_Loreto/Arduino/rs485-Full/lib/LnFunctions
    rm -f /home/pi/485d_Loreto/Arduino/rs485-Full/lib/RS485_protocol
}
# -------------------------------------------------------------------------
if [[ "$action" == "" ]]; then
    echo "ERRORE: nessuna azione richiesta!"
    echo ""
    echo "Immettere l'azione da eseguire (o piu' di una)"
    echo " senza spazi intermedi, in questa sequenza:"
    echo "      c -> clear"
    echo "      b -> build"
    echo "      u -> upload"
    echo "      s -> read serial"
    echo ""
    echo "esempio:   bash pioMakeSteve.sh cbu 0"
    echo " esegue clear build upload su  /dev/ttyUSB0"
    exit
fi

#echo $action
dev="/dev/ttyUSB"$device
#echo "$dev"

#cleam -------------------------------------------------------------------------
if [[ "$action" =~ "c" ]]; then
    echo "------"
    echo "CLEAN"
    echo "------"
    echo
    platformio run --target clean
fi

#build -------------------------------------------------------------------------
#creaLink              # asteriscato
if [[ "$action" =~ "b" ]]; then
    echo "------"
    echo "BUILD"
    echo "------"
    echo
    platformio run  --environment nano
fi

#upload ------------------------------------------------------------------------
if [[ "$action" =~ "u" ]]; then
    echo "------"
    echo "UPLOAD"
    echo "------"
    echo
    if [[ "$device" = "" ]]; then
        echo "Attenzione, per il comando u upload serve "
        echo " inserire il nr del device (es: 0 per USB0). "
        echo " Il comando sara' pioMakeSteve.sh u 0"
        exit
    fi
    platformio run --target upload --upload-port $dev
fi
#deleteLink              # asteriscato

#monitor -----------------------------------------------------------------------
if [[ "$action" =~ "s" ]]; then
    echo "------"
    echo "MONITOR"
    echo "------"
    echo
    if [[ "$device" = "" ]]; then
        echo "Attenzione, per il comando s monitor serve "
        echo " inserire il nr del device (es: 0 per USB0). "
        echo " Il comando sara' pioMakeSteve.sh s 0"
        exit
    fi
    platformio device monitor -p $dev
fi

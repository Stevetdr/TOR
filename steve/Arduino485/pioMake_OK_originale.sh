#!/bin/bash



# viene solo caricata e verrà richiamata dai vari scripts
function Esegui() {
    cmd=$*
    echo "executing: $cmd"
    eval $cmd
    rCode=$?
    [[ ! "$rCode" == "0" ]] && echo "E R R O R    on $CMD" && exit $rCode
}



# ########################################################################
# # Creazione dei LINK logici per inserire le directory con le librerie.
# ########################################################################
function createLibraryLink {

    for dirName in $dirNames; do
        sourceDir="${LORETO_LIB_DIR}/${dirName}"
        linkDir="${Project_LIB_DIR}/${dirName}"

         # ---- create new link
        if [ -d "$sourceDir" ]; then
            Esegui "ln -s ${sourceDir} ${linkDir}"
        else
            echo "skipping      .....  $dirName"
        fi
    done

     # ---- create new link
    if [ -f "$platformioINI" ]; then
        Esegui "ln -s ${platformioINI} ${platformioLNK}"
    fi


}

function clearLibraryLink {
    echo "removing LINKs to my libraries"
    for dirName in $dirNames; do
        linkDir="${Project_LIB_DIR}/${dirName}"
        if [ -L "$linkDir" ]; then
            Esegui "rm -f ${linkDir}";
        fi
    done
    if [ -L "${platformioLNK}" ]; then
        Esegui "rm -f ${platformioLNK}";
    fi
}

# ########################################################################
# # M A I N
# ########################################################################
    thisDir="$(dirname  "$(test -L "$0" && readlink "$0" || echo "$0")")"     # risolve anche eventuali LINK presenti sullo script
    thisDir=$(cd $(dirname "$thisDir"); pwd -P)/$(basename "$thisDir")        # GET AbsolutePath
    scriptDir=${thisDir%/.*}

    thisDir="$(test -L "$PWD" && readlink "$PWD" || echo "$PWD")"     # risolve anche eventuali LINK presenti sullo script
    thisDir=$(cd $(dirname "$thisDir"); pwd -P)/$(basename "$thisDir")        # GET AbsolutePath
    currentDir=${thisDir%/.*}


    GIT_ROOT_DIR="$scriptDir"
    Project_DIR="$currentDir"

    Project_LIB_DIR="$Project_DIR/lib"
    LORETO_LIB_DIR="$GIT_ROOT_DIR/LnLibraries"

    platformioINI="$GIT_ROOT_DIR/platformio.ini"
    platformioLNK="${Project_DIR}/platformio.ini"

    # LORETO_LIB_DIR="/home/pi/GIT-REPO/HW-Projects/LnProtocol/LnLibraries"

    echo
    echo "      GIT ROOT DIR    dir is: $GIT_ROOT_DIR"
    echo "      library         dir is: $LORETO_LIB_DIR"
    echo
    echo "      current         dir is: $currentDir"
    echo "      prjLib          dir is: $Project_LIB_DIR"
    echo
    echo "      platformioINI   is: $platformioINI"
    echo "      platformioLNK   is: $platformioLNK"
    echo

    device=$2
    device=/dev/${2}
    action=$1

    if [[ ! -d $Project_LIB_DIR ]]; then
        echo
        echo "  ERROR directory: $Project_LIB_DIR doesn't exists."
        echo
        exit 1
    fi

    if [[ ! -d $LORETO_LIB_DIR ]]; then
        echo
        echo "  ERROR directory: $LORETO_LIB_DIR doesn't exists."
        echo
        exit 1
    fi

    if [[ ! "$action" =~ [cbus] ]]; then
        echo "      immettere uno o più dei seguenti parametri:"
        echo "          c = clean"
        echo "          b = build"
        echo "          u = upload"
        echo "          s = serial read"
        echo
        exit
    fi


    dirNames='RCSwitch LnFunctions RS485_protocol VirtualWire115 MAX7219 Adafruit-ST7735-Library Adafruit_GFX'
    dirNames='LnFunctions RS485_protocol'
    VERBOSE='--verbose'
    VERBOSE=''

    clearLibraryLink
    createLibraryLink

    if [[ "$action" =~ "v" ]]; then
        echo "setting VERBOSE option"
        VERBOSE='--verbose'
    fi


    if [[ "$action" =~ "c" ]]; then
        echo "Cleaning project ....: $PWD"
        Esegui "platformio run $VERBOSE --target clean"
    fi

    if [[ "$action" =~ "b" ]]; then
        echo
        echo "Building .... from dir: $PWD"
        Esegui "platformio run $VERBOSE --environment nano"
    fi

    if [[ "$action" =~ "u" ]]; then
        [[ "$device" == "" ]] && echo "Enter USB port: ttyUSB0, ttyUSB1, slave01, ...." && exit
        [[ ! -c "$device" ]] && echo "$device is NOT a character device" && exit
        Esegui "platformio run $VERBOSE --target upload --upload-port $device"
    fi

    # set -x
    clearLibraryLink

    if [[ "$action" =~ "s" ]]; then
        [[ "$device" == "" ]] && echo "Enter USB port: /dev/ttyUSB0, /dev/ttyUSB1, ...." && exit
        [[ ! -c "$device" ]] && echo "$device is NOT a character device" && exit
        Esegui "platformio device monitor -p $device"
    fi

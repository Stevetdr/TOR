#!/bin/bash

    isActive=$(ps -ef | grep pigpiod | grep -v grep | wc -l)
    [[ "$isActive" -ne "1" ]] && sudo pigpiod -a1


    thisDir="$(dirname  "$(test -L "$0" && readlink "$0" || echo "$0")")"     # risolve anche eventuali LINK presenti sullo script
    thisDir=$(cd $(dirname "$thisDir"); pwd -P)/$(basename "$thisDir")        # GET AbsolutePath
    baseDir=${thisDir%/.*}                                                      # Remove /. finale (se esiste)
    parentDir=${baseDir%/bin}                                               # Remove /bin finale (se esiste)
    pythonDir=${baseDir}/python                                               # Remove /bin finale (se esiste)
    # echo $parentDir
    MAIN_PY="$pythonDir/__main__.py"
    MAIN_ZIP="$pythonDir/LN-Protocol.zip"

    # parentDir="$(dirname $baseDir)"
    # sourceDir="${parentDir}/SOURCE"

    if [ -f "$MAIN_PY"  ]; then
        mainProgram="$MAIN_PY"
    else
        mainProgram="$MAIN_ZIP"
    fi

        # "$@" importante per passare parametri con BLANK
    python3 $mainProgram ttyUSB2 "$@"

    # python3 ./__main__.py ttyUSB2

#!/bin/bash
RETEST=NO

POSITIONAL=()
while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -r|--retest)
    printf "\nRETEST - skip compilation\n\n"
    RETEST=YES
    shift # past argument
    shift # past value
    ;;
    *)    # unknown option
    POSITIONAL+=("$1") # save it in an array for later
    shift # past argument
    ;;
esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters
if [[ -n $1 ]]; then
    device=$1
fi

## COMPILE ALL EXAMPLES
if [ $RETEST != YES ]; then
  printf "\nCompiling all examples"
  for path in examples/*/ ; do
    directoryName=$(basename $path)
    printf "\n\nCompiling %s to bin/%s.bin" $path $directoryName
    particle compile p ./$path --saveTo bin/$directoryName.bin
    if [ $? -eq 0 ]; then
      printf "\n\nSUCCESS: created bin/%s.bin" $directoryName
    else
      printf "\n\nFAIL: unable to compile bin for \"%s\"\n" $directoryName
      break;
    fi
  done
fi

## FLASH ALL EXAMPLES AND TEST
printf "Flashing examples\n\n"
for bin in bin/* ; do
  printf "\n\n---%s---\n" $bin
  particle flash $device $bin
  if [ $? -eq 0 ]; then
    printf "\n\nSUCCESS: Flashed %s to %s" $bin $1
  else
    printf "\n\nFAIL: unable to flash %s to %s" $bin $1
    #break;
  fi
  printf "\n_______________________\n"
  printf "Starting Serial Monitor\n"
  printf "Press \" CTRL + \\ \" to exit serial monitor\n"
  printf "and start next test"
  printf "\n_______________________\n"
  particle serial monitor
done

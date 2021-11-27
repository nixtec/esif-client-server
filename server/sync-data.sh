#!/bin/sh


function dataview() {
  return 0
}

function notify_client() {
  host=$1
  port=$2
  shift 2
  echo "DATA ARRIVED $@" > /dev/tcp/${host}/${port}
  return 0
}

PORT=9999

if [ "$USER" = "ayub" ] ; then
  CLIENT_IP="127.0.0.1"
else
  CLIENT_IP="192.168.20.2"
fi


# notify client machine when new data arrived
LOCATION="/var/regform/bdb/Rajshahi/2005"

trap "echo Signal Caught, Exiting...; exit 0" SIGINT SIGQUIT SIGTERM SIGTERM

OLDDIR=`pwd`

cd "$LOCATION"

declare -a PREV_STATE
declare -a NEXT_STATE

while [ 1 ] ; do
  filelist=`ls -1`
  i=0
#  echo "Previous >>"
#  echo ${filelist}
  for file in ${filelist} ; do
#    echo "$file"
    PREV_STATE[i]=`stat ${file} --format=%Y`
    i=$((i+1))
  done
  sleep 1
  i=0
#  listfile=`ls`
#  echo "Next >>"
#  echo $listfile
  for f in ${filelist} ; do
#    echo "$f"
    NEXT_STATE[i]=`stat ${f} --format=%Y`
    if [ ${PREV_STATE[i]} -ne ${NEXT_STATE[i]} ] ; then
#      echo "${PREV_STATE[i]}"
#      echo "${NEXT_STATE[i]}"
#      echo "Modified"
      dataview
      notify_client "$CLIENT_IP" "$PORT" "IN $f"
    fi
    i=$((i+1))
  done

done


exit $?

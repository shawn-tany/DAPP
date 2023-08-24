BRANCH=$(git rev-parse --abbrev-ref HEAD)

VERSION=${BRANCH}-$(git log --pretty=oneline -1 |cut -c 1-8)

MODIFYS="A M D R U"

STATUSS=$(git status -s |awk '{print substr($0,0,2)}')

for status in ${STATUSS};
do
    for modify in ${MODIFYS};
    do
        if [ "${status}" = "${modify}" ]
        then
            VERSION=${VERSION}-${status}
        fi
    done
done

NOPUSH=`git log ${BRANCH} ^origin/${BRANCH} | grep commit | wc -l`

if [ ${NOPUSH} -gt 0 ];
then
    VERSION=${VERSION}-NOPUSH${NOPUSH}
fi

echo ${VERSION}

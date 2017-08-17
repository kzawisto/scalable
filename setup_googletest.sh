
SCRIPT=`realpath $0`
SCRIPTPATH=`dirname $SCRIPT`
cd $SCRIPTPATH
rm -rf "$SCRIPTPATH/googletest"

git clone https://github.com/google/googletest
git checkout $(git tag -l | tail -n 1)

cd googletest
cmake -DCMAKE_INSTALL_PREFIX:PATH=$SCRIPTPATH . 
make -j4 install

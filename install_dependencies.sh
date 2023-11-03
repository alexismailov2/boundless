#from inside the root directory of the project
# Run system update
sudo apt-get update -y
sudo apt-get install -y apt-utils software-properties-common

# Install required packages
sudo add-apt-repository -y ppa:ubuntuhandbook1/ppa
sudo apt-get update -y
sudo apt-get install -y build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3 
libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev 
libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libboost-all-dev 
software-properties-common git libminiupnpc-dev libzmq3-dev libqt4-dev wget unzip qt4-qmake && sudo apt-get install git && sudo apt-get install build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils && sudo apt-get install libboost-system-dev libboost-filesystem-dev libboost-chrono-dev libboost-program-options-dev libboost-test-dev libboost-thread-dev libboost-all-dev software-properties-common  libminiupnpc-dev && sudo apt-get install libzmq3-dev && sudo apt-get install libqt5gui5 libqt5core5a libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler && sudo apt-get install libqt4-dev libprotobuf-dev protobuf-compiler

# Download libdb4.8 sources
wget http://download.oracle.com/berkeley-db/db-4.8.30.zip
unzip db-4.8.30.zip

# Step into libdb build directory
cd db-4.8.30/build_unix

# Fix overwrite of gcc builtin
sed -i 's/__atomic_compare_exchange/db_atomic_compare_exchange_db/g' ../dbinc/atomic.h
# Download libdb4.8 sources
wget http://download.oracle.com/berkeley-db/db-4.8.30.zip
unzip db-4.8.30.zip

# Step into libdb build directory
cd db-4.8.30/build_unix

# Fix overwrite of gcc builtin
sed -i 's/__atomic_compare_exchange/db_atomic_compare_exchange_db/g' ../dbinc/atomic.h

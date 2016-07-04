wget http://download.oracle.com/otn-pub/java/jdk/7u65-b17/jdk-7u65-linux-x64.tar.gz
tar xvzf jdk-7u65-linux-x64.gz
cd /usr/local && sudo ln -s jdk1.7.0_65 jdk
cd /usr/local/bin && sudo ln -s /usr/local/jdk/bin/java

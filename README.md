# Zephyr Device Tree Examples

## Quick Start

Step 1: Install Zephyr

```
$ pip install west
$ west init ~/zephyrproject
$ cd ~/zephyrproject
$ west update
$ west zephyr-export
$ pip install -r ~/zephyrproject/zephyr/scripts/requirements.txt
$ echo "source ~/zephyrproject/zephyr/zephyr-env.sh" >> ~/.bashrc
```

Step 2: Install Zephyr-SDK

```
$ cd ~
$ wget https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/zephyr-sdk-0.16.8_linux-x86_64.tar.xz
$ wget -O - https://github.com/zephyrproject-rtos/sdk-ng/releases/download/v0.16.8/sha256.sum | shasum --check --ignore-missing
$ tar xvf zephyr-sdk-0.16.8_linux-x86_64.tar.xz
$ cd zephyr-sdk-0.16.8
$ ./setup.sh

$ sudo cp ~/zephyr-sdk-0.16.8/sysroots/x86_64-pokysdk-linux/usr/share/openocd/contrib/60-openocd.rules /etc/udev/rules.d
$ sudo udevadm control --reload-rules
$ sudo udevadm trigger
```

Step 3: Magnetometer Example

```
$ git clone https://github.com/wuhanstudio/bbc-microbit-v2-zephyr-bsp
$ cd bbc-microbit-v2-zephyr-bsp 
$ cd hello-lsm303agr

$ source ~/zephyrproject/zephyr/zephyr-env.sh
$ west build -b bbc_microbit_v2
$ west flash
```


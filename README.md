# CSCI5103_Project4
Project 4 - Linux Device Drivers

- Bryan Baker - bake1358@umn.edu
- Alice Anderegg - and08613@umn.edu
- Hailin Archer - deak0007@umn.edu

## How To Run
To make the module and the test files, insert the module, run the default tests, and remove the module, run:

```
chmod +x buildandrun.sh
./buildandrun.sh
```

Enter password for sudo if required.

The default test will run prodcons at three levels:
- 1 producer and 1 consumer each producing/consuming 1 item
- 10 producers and 10 consumers each producing/consuming 10 items
- 100 producers and 100 consumers each producing/consuming 100 items

Logs are deposited in 'test/prodcons_run1', 'test/prodcons_run10', and 'test/prodcons_run100 '.

To run further tests, from the test directory run:

```
 ./prodcons [# of producers] [# of producer items] [# of consumers] [# of consumer items]
```

The logs will appear in the test directory.

## Updates done to the VM to enable initial builds

- Created a soft link to the correct arch folder
```
sudo ln -s /lib/modules/4.15.0-72-generic/build/arch/arm64/include/asm /lib/modules/4.15.0-72-generic/build/include/asm
```

- Updated scullBuffer.h to use <asm/switch_to.h> rather than <asm/system.h>

- Updated scullBuffer.h to use <linux/uaccess.h> rather than <asm/uaccess.h>

- Updated scullBuffer.c to use raw_copy_from_user rather than copy_from_user

- Updated scullBuffer.c to use raw_copy_to_user rather than copy_to_user

- Installed Python:
```
sudo apt-get python
```

- Installed and configured git:
```
sudo install git
git config --global http.proxy http://proxy.cselabs.umn.edu:8888
git config --global https.proxy https://proxy.cselabs.umn.edu:8888
```


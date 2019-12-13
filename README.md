# CSCI5103_Project4
Project 4 - Linux Device Drivers

- Bryan Baker - bake1358@umn.edu
- Alice Anderegg - and08613@umn.edu
- Hailin Archer - deak0007@umn.edu

## How To Run
To make the module and the test files, insert the module, run the default tests, and remove the module, run:
./buildandrun.sh

Enter password for sudo if required.

The default test will run prodcons at three levels:
- 1 producer and 1 consumer each producing/consuming 1 item
- 10 producers and 10 consumers each producing/consuming 10 items
- 100 producers and 100 consumers each producing/consuming 100 items

Logs are deposited in 'test/prodcons_run1', 'test/prodcons_run10', and 'test/prodcons_run100 '.

To run further tests, from the test directory run:
 ./prodcons [# of producers] [# of producer items] [# of consumers] [# of consumer items]

The logs will appear in the test directory.

This test is implementing the real-time kinematic-based foot-strike detection algorithm F-VESPA using kinematic data stored in a .txt file. 
This test invokes two processes: one for the implementation of the F-VESPA algorithm and one for loading the kinematic data from the .txt file.
The kinematic data are loaded to a shared memory, through which the other process can access them and apply the F-VESPA algorithm. 
This test can run in any computer and there are no dependencies to other software. 
This test is implementing the real-time kinematic-based foot-strike detection algorithm F-VESPA using kinematic data streamed by Vicon Nexus. 
This test invokes two processes: one for the implementation of the F-VESPA algorithm and one for receiving the streaming kinematic data from Vicon Nexus.
The kinematic data are loaded to a shared memory, through which the other process can access them and apply the F-VESPA algorithm for both feet. 
This test can run in any computer, but the software "Vicon Nexus" needs to run as well. 
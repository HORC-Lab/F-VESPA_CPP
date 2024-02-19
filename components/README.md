# Components Readme

"Components" are classes that implement functionality like hardware interface or algorithms. These classes 
encapsulate their functionality (e.g. vsm control) and provide an interface that makes sense for our
application (e.g. methods like set_stiffness, home_motors, and get_position). These components can also 
contain a set of test functionality that can be conditionally compiled using compile flags, allowing us
to do testing outside of the shared memory framework.

### Component Requirements
- Encapsulates all its nitty gritty functionality
- Has a `.h` header that defines the interface for the class and a `.cpp` file that contains the implementation 
- Does not excecute any code when compiled normally (eg. without special flags) 



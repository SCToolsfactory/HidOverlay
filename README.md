# HID Overlay

HID Layer to hide HID devices from detection by the calling program

Essentially only designed to help when consolidating game devices into new virtual devices and then hiding the ones consolidated to either not confuse or if by chance only e.g. 4 devices are supported...
e.g. for games using DXinput - for others... you may just try.

Risk Analysis ...
* There is no installation required
* The procedure is applied on the user level
* It impacts only the application where the Overlay is deployed 
* Without blacklist file there is no impact whatsoever
* Reverting completely by deleting the added DLL module


NOTE: THIS _ IS _ VERY _ EARLY _ WORK _ IN _ PROGRESS _ IT _ MAY _ JUST _ BREAK _ AT _ ANY _ TIME ;-)

# Design and Implementation

Implements a HID.dll layer to filter devices from detection.
Propagates all HID calls to the original HID.DLL which is loaded from the Win SystemDirectory.
Intercepts the call `HidD_GetAttributes` and returns FALSE for blacklisted devices.
Effecting the caller to ignore the device as it is not a legit HID device with properties.

The HidOverlay.dll is architecture dependent and can be build for Win32 and x64 flavors.
In general the x64 build is appropriate for modern games.
Dropping the wrong version will cause the DLL to have no effect as it is not loaded and used.

Droping the DLL into the applications directory to make use of the search strategy for modules (if a module is not loaded with a path)  
   
    1 _The directory containing the executable file_        - > > we use this one to overlay HID.DLL
    2 _The Windows system directory returned by GetWindowsDirectory_
    3 _The 16-bit system directory—that is, the System subfolder under the Windows directory_
    4 _The Windows directory returned by GetSystemDirectory_
    5 _The process' current directory_
    6 _The directories listed in the PATH environment variable_


This overlay, renamed to Hid.dll, when loaded by the caller reads a file 'HidOverlay.blist' from the callers **current directory** this might be the application (exe) directory but might be another one which one has to find out (sorry for that one..) and collects devices as 'to be ignored' aka Blacklisted. 
 
The format of the blacklist file is a line based text with lines such as: 
 
`vid pid comment`  where comment is ignored and only to remember what device it is
 
e.g.  
`0x06a3 0x0b6a Saitek X-65F HOTAS`  
`0x0738 0x2215 Saitek X55 Joystick`  

Blist file checking is **NONE** - start in the first line and an NL at the end. 
The maximum number of devices to collect is defined in BLACKLIST_SIZE (10)
 
# Usage

See also the 'Docs' directory for notes on games where it was applied


In order to use the HIDOverlay : 

Just within the target application Exe folder:

*Drop 'HidOverlay.dll' and rename it to 'Hid.dll'  
*Add a plain ASCII text file 'HidOverlay.blist' with the devices to be hidden  
*Without such a file there is no filtering of HID devices whatsoever.  
**Note: the 'HidOverlay.blist' file is read from the applications **current directory** and the calling application may change the current directory away from where the Exe is - this is kind of experimenting where to put the blist file**  
  
*To remove the support - just delete the Hid.dll from the application directory or rename it back to 'HIDOverlay.dll'<br>


# Edge-based-knowledge-inference-for-intelligent-vehicular-networks
The objective of this project is to reduce the amount of data transferred between vehicles and Road-Side Units (RSUs), so as to reduce the processing power required at the RSUs and make it possible for the architecture to perform the required calculations irrespective of the network strength.

NOTE: working with ns-3.30.1 (can't say if others will work)

To Run the Application copy the Custom sumo folder to /ns-allinone-3.30.1/ns-3.30.1/scratch/ folder
Then go to /ns-allinone-3.30.1/ns-3.30.1/ and run the command
./waf --run CustomSumo > abc.out 2>&1
THere will be an output file abc.out in the /ns-allinone-3.30.1/ns-3.30.1/ folder with the logs
To run the visualization go to /ns-allinone-3.30.1/ns-3.30.1/ and run the command
./waf --run CustomSumo --vis
and simulate 

You can add any mobility.tcl file you want just make sure that you add the RSUs.

After adding the RSUs in the .tcl File run the simulation in ns3 and you will see the MAC address of the nodes (if you dont see then follow the format in the pdf.

Add the MAC address in the custom-application.cc file and change the number of nodes in the beginning and the addition factor in the broadcast function for the sent metric(equal to the number of RSUs you have added)
Next add the SendX function in the appropriate part of the custom-application 
The mobility.tcl file that is being used rn is a snipped version of the complete file. 
To get the complete mobility file follow the tutorials online(https://www.youtube.com/watch?v=i5dRnaG707c&t=1s)
The OSM/SUMO files are present in your dated folder and can be used too

PS:If you dont get NS3 and/or SUMO spend a week just looking at the screen. It really helps.

PPS: the application base was taken from https://www.youtube.com/channel/UC8GCZM8z4DW4DkaNHtzfpBg


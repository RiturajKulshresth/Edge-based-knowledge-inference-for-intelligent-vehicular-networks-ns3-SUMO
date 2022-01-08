#include "ns3/wave-module.h"
#include "ns3/wifi-module.h"
#include "ns3/mobility-module.h"
#include "ns3/core-module.h"
#include "custom-application.h"
#include "wave-setup.h"
#include "ns2-node-utility.h"

using namespace ns3;
NS_LOG_COMPONENT_DEFINE ("CustomApplicationExample");

int main (int argc, char *argv[])
{
  CommandLine cmd;

  std::string mobility_file = "scratch/CustomSumo/ns2mobility.tcl";
  Ns2NodeUtility ns2_utility (mobility_file);

  //Number of nodes
  uint32_t nNodes = ns2_utility.GetNNodes();
  double simTime = ns2_utility.GetSimulationTime(); //4 seconds
  double interval = 0.1;
  bool enablePcap = false;
  cmd.AddValue ("t","Simulation Time", simTime);
  cmd.AddValue ("i", "Broadcast interval in seconds", interval);
  cmd.AddValue ("n", "Number of nodes", nNodes);
  cmd.AddValue ("pcap", "Enable PCAP", enablePcap);
  cmd.Parse (argc, argv);

  NodeContainer nodes;
  nodes.Create(nNodes);

  //LogComponentEnable ("CustomApplication", LOG_LEVEL_FUNCTION);
  /*
    You must setup Mobility. Any mobility will work. Use one suitable for your work
  */

  Ns2MobilityHelper sumo_trace (mobility_file);
  sumo_trace.Install();
 
  WaveSetup wave;
  NetDeviceContainer devices = wave.ConfigureDevices(nodes);
  
  //Create Application in nodes
  
  for (uint32_t i=0; i<nodes.GetN(); i++)
  {
    Ptr<CustomApplication> app_i = CreateObject<CustomApplication>();
    app_i->SetBroadcastInterval (Seconds(interval));
    app_i->SetStartTime (Seconds (0));
    app_i->SetStopTime (Seconds (simTime));
    nodes.Get(i)->AddApplication (app_i);
  }

  Simulator::Stop(Seconds(simTime));
  Simulator::Run();

  std::cout<<"\n"<<nNodes<<" "<<simTime<<"\n";
  ns2_utility.PrintInformation();
  Simulator::Destroy();
}

#include "ns3/mobility-model.h"
#include "ns3/log.h"
#include "ns3/simulator.h"
#include "custom-application.h"
#include "custom-data-tag.h"
#include "ns3/random-variable-stream.h"
#include "ns3/core-module.h"
#include "ns3/wave-module.h"

#define RED_CODE "\033[91m"
#define GREEN_CODE "\033[32m"
#define END_CODE "\033[0m"
#define NUM_NODES 50

int itrcount = 0;
int sentcount = 0;
int sentVcount = 0;
int sentPcount = 0;
int totalcount = 0;
ns3::Address addR;
namespace ns3
{
    NS_LOG_COMPONENT_DEFINE("CustomApplication");
    NS_OBJECT_ENSURE_REGISTERED(CustomApplication);

    TypeId CustomApplication::GetTypeId()
    {
        static TypeId tid = TypeId("ns3::CustomApplication")
                    .SetParent <Application> ()
                    .AddConstructor<CustomApplication> ()
                    .AddAttribute ("Interval", "Broadcast Interval",
                    TimeValue (MilliSeconds(100)),
                    MakeTimeAccessor (&CustomApplication::m_broadcast_time),
                    MakeTimeChecker());
        return tid;
    }

    TypeId CustomApplication::GetInstanceTypeId() const
    {
        return CustomApplication::GetTypeId();
    }

    CustomApplication::CustomApplication()
    {
        m_broadcast_time = MilliSeconds (100); //every 100ms
        m_packetSize = 1000; //1000 bytes
        // m_time_limit = Seconds (15);
        // m_mode = WifiMode("OfdmRate6MbpsBW10MHz");
    }
    CustomApplication::~CustomApplication(){}


    void CustomApplication::StartApplication()
    {
        NS_LOG_UNCOND (this);
        Packet::EnablePrinting();
        //Set A Receive callback
        Ptr<Node> n = GetNode ();

        for (uint32_t i = 0; i < n->GetNDevices (); i++)
        {
            Ptr<NetDevice> dev = n->GetDevice (i);
            if (dev->GetInstanceTypeId () == WaveNetDevice::GetTypeId())
            {
                m_waveDevice = DynamicCast <WaveNetDevice> (dev);
                //ReceivePacket will be called when a packet is received
                dev->SetReceiveCallback (MakeCallback (&CustomApplication::ReceivePacket, this));
                /*
                If you want promiscous receive callback, connect to this trace. 
                For every packet received, both functions ReceivePacket & PromiscRx will be called. with PromicRx being called first!
                */
                Ptr<WifiPhy> phy = m_waveDevice->GetPhys()[0]; //default, there's only one PHY in a WaveNetDevice
                phy->TraceConnectWithoutContext ("MonitorSnifferRx", MakeCallback(&CustomApplication::PromiscRx, this));
                break;
            } 
        }
        if (m_waveDevice)
        {
            //Let's create a bit of randomness with the first broadcast packet time to avoid collision
            Ptr<UniformRandomVariable> rand = CreateObject<UniformRandomVariable> ();
            Time random_offset = MicroSeconds (rand->GetValue(50,200));
            Simulator::Schedule (m_broadcast_time+random_offset, &CustomApplication::BroadcastInformation, this);
        }
        else
        {
            NS_FATAL_ERROR ("There's no WaveNetDevice in your node");
        }  
    }

    void CustomApplication::SetBroadcastInterval (Time interval)
    {
        NS_LOG_UNCOND (this << interval);
        m_broadcast_time = interval;
    }

    // CustomDataTag ptag;
    Vector posList[NUM_NODES];
    Vector velList[NUM_NODES];
    int lastsent[NUM_NODES] = { 0 };
    float velavgx = 0;
    float velavgy = 0;
    float velavgz = 0;

    void CustomApplication::BroadcastInformation()
    {
        NS_LOG_UNCOND (this);
        //Setup transmission parameters
        TxInfo tx;
        tx.channelNumber = CCH; 
        // tx.preamble = WIFI_PREAMBLE_LONG;
        tx.priority = 7; //highest priority.
        tx.txPowerLevel = 7;
        tx.dataRate = WifiMode("OfdmRate6MbpsBW10MHz");
        // tx.dataRate = m_mode;
        // Mac48Address addrs="02-06-00:00:00:00:00:01";
        // Mac48Address addrs1="02-06-00:00:00:00:00:03";
        // Mac48Address addrs2="02-06-00:00:00:00:00:0d";
        // Mac48Address addrs434="02-06-00:00:00:00:01:b3";
        // Mac48Address addrs435="02-06-00:00:00:00:01:b4";
        // Mac48Address addrs436="02-06-00:00:00:00:01:b5";
        // Mac48Address addrs437="02-06-00:00:00:00:01:b6";
        // Mac48Address addrs438="02-06-00:00:00:00:01:b7";
        // Mac48Address addrs439="02-06-00:00:00:00:01:b8";
        // Mac48Address addrs440="02-06-00:00:00:00:01:b9";
        // Mac48Address addrs441="02-06-00:00:00:00:01:ba";
        // Mac48Address addrs442="02-06-00:00:00:00:01:bb";
        // Mac48Address addrs443="02-06-00:00:00:00:01:bc";
        // Mac48Address addrs444="02-06-00:00:00:00:01:bd";
        // Mac48Address addrs445="02-06-00:00:00:00:01:be";
        // Mac48Address addrs446="02-06-00:00:00:00:01:bf";
        // Mac48Address addrs447="02-06-00:00:00:00:01:c0";
        // Mac48Address addrs448="02-06-00:00:00:00:01:c1";
        // Mac48Address addrs449="02-06-00:00:00:00:01:c2";
        // Mac48Address addrs450="02-06-00:00:00:00:01:c3";
        Mac48Address addrs14="02-06-00:00:00:00:00:0f";
        Mac48Address addrs15="02-06-00:00:00:00:00:10";
        Mac48Address addrs16="02-06-00:00:00:00:00:11";
        Mac48Address addrs17="02-06-00:00:00:00:00:12";
        Mac48Address addrs18="02-06-00:00:00:00:00:13";
        Mac48Address addrs19="02-06-00:00:00:00:00:14";
        Mac48Address addrs20="02-06-00:00:00:00:00:15";
        Mac48Address addrs21="02-06-00:00:00:00:00:16";
        Mac48Address addrs22="02-06-00:00:00:00:00:17";
        Mac48Address addrs23="02-06-00:00:00:00:00:18";
        Mac48Address addrs24="02-06-00:00:00:00:00:19";
        Mac48Address addrs25="02-06-00:00:00:00:00:1a";
        Mac48Address addrs26="02-06-00:00:00:00:00:1b";
        Mac48Address addrs27="02-06-00:00:00:00:00:1c";
        Mac48Address addrs28="02-06-00:00:00:00:00:1d";
        Mac48Address addrs29="02-06-00:00:00:00:00:1e";
        Mac48Address addrs30="02-06-00:00:00:00:00:1f";
        Mac48Address addrs31="02-06-00:00:00:00:00:20";
        Mac48Address addrs32="02-06-00:00:00:00:00:21";
        Mac48Address addrs33="02-06-00:00:00:00:00:22";
        Mac48Address addrs34="02-06-00:00:00:00:00:23";
        Mac48Address addrs35="02-06-00:00:00:00:00:24";
        Mac48Address addrs36="02-06-00:00:00:00:00:25";
        Mac48Address addrs37="02-06-00:00:00:00:00:26";
        Mac48Address addrs38="02-06-00:00:00:00:00:27";
        Mac48Address addrs39="02-06-00:00:00:00:00:28";
        Mac48Address addrs40="02-06-00:00:00:00:00:29";
        Mac48Address addrs41="02-06-00:00:00:00:00:2a";
        Mac48Address addrs42="02-06-00:00:00:00:00:2b";
        Mac48Address addrs43="02-06-00:00:00:00:00:2c";
        Mac48Address addrs44="02-06-00:00:00:00:00:2d";
        Mac48Address addrs45="02-06-00:00:00:00:00:2e";
        Mac48Address addrs46="02-06-00:00:00:00:00:2f";
        Mac48Address addrs47="02-06-00:00:00:00:00:30";
        Mac48Address addrs48="02-06-00:00:00:00:00:31";
        Mac48Address addrs49="02-06-00:00:00:00:00:32";

        if (itrcount == NUM_NODES) 
        {
            itrcount = 0;
        }

        Ptr<Packet> packet = Create <Packet> (m_packetSize);
        //let's attach our custom data tag to it
        CustomDataTag tag;
        tag.SetNodeId ( GetNode()->GetId() );
        Vector pos = GetNode()->GetObject<MobilityModel>()->GetPosition();
        Vector vel = GetNode()->GetObject<MobilityModel>()->GetVelocity();
        tag.SetPosition (pos);
        tag.SetVelocity (vel);
        //timestamp is set in the default constructor of the CustomDataTag class as Simulator::Now()
        
        velavgx += fabs(vel.x);
        velavgy += fabs(vel.y);
        velavgz += fabs(vel.z);

        NS_LOG_UNCOND("\tPrevious pos: " << posList[itrcount] << "\tCurrent pos: " << pos);
        NS_LOG_UNCOND("\tDIfference: " << fabs(posList[itrcount].x - pos.x));
        //attach the tag to the packet
        packet->AddPacketTag (tag);
        // Broadcast the packet as WSMP (0x88dc)
        if (fabs(posList[itrcount].x - pos.x) > 4.83 || fabs(posList[itrcount].y - pos.y) > 4.83 || fabs(posList[itrcount].z - pos.z) > 4.83 || lastsent[itrcount]>5) 
        {
            // m_waveDevice->SendX (packet, addrs434, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs435, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs436, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs437, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs438, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs439, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs440, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs441, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs442, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs443, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs444, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs445, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs446, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs447, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs448, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs449, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs450, 0x88dc, tx);
            // m_waveDevice->SendX (packet, addrs, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs1, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs2, 0x88dc, tx);
            // m_waveDevice->SendX (packet, Mac48Address::GetBroadcast(), 0x88dc, tx);

            m_waveDevice->SendX (packet, addrs14, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs15, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs16, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs17, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs18, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs19, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs20, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs21, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs22, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs23, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs24, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs25, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs26, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs27, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs28, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs29, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs30, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs31, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs32, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs33, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs34, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs35, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs36, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs37, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs38, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs39, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs40, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs41, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs42, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs43, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs44, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs45, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs46, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs47, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs48, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs49, 0x88dc, tx);

            sentcount+=36;
            sentPcount += 36;
            lastsent[itrcount]=0;

        }
        else if (fabs(velList[itrcount].x - vel.x) > 0.26 || fabs(velList[itrcount].y - vel.y) > 0.26 || fabs(velList[itrcount].z - vel.z) > 0.26 || lastsent[itrcount] == 5) 
        {
            // m_waveDevice->SendX (packet, addrs434, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs435, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs436, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs437, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs438, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs439, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs440, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs441, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs442, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs443, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs444, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs445, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs446, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs447, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs448, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs449, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs450, 0x88dc, tx);
            // m_waveDevice->SendX (packet, addrs, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs1, 0x88dc, tx);
            // m_waveDevice->SendXe (packet, addrs2, 0x88dc, tx);
            // m_waveDevice->SendX (packet, Mac48Address::GetBroadcast(), 0x88dc, tx);

            m_waveDevice->SendX (packet, addrs14, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs15, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs16, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs17, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs18, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs19, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs20, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs21, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs22, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs23, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs24, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs25, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs26, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs27, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs28, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs29, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs30, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs31, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs32, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs33, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs34, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs35, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs36, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs37, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs38, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs39, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs40, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs41, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs42, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs43, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs44, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs45, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs46, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs47, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs48, 0x88dc, tx);
            m_waveDevice->SendXe (packet, addrs49, 0x88dc, tx);

            sentcount+=36;
            sentVcount += 36;
            lastsent[itrcount] = 0;

        }
        else
        {
            lastsent[itrcount]++;
        }
        posList[itrcount] = pos;
        itrcount++;
        totalcount+=36;
        NS_LOG_UNCOND ("\nSent: " << sentcount << " out of " << totalcount); 
        NS_LOG_UNCOND ("Sent due to velocity: " << sentVcount << " out of " << totalcount); 
        NS_LOG_UNCOND ("Sent due to position: " << sentPcount << " out of " << totalcount);

        if (totalcount == 125000) {
            velavgx /= totalcount;
            velavgy /= totalcount;
            velavgz /= totalcount;

            NS_LOG_UNCOND ("\tAvg velx: " << velavgx << "\tAvg vely: " << velavgy << "\tAvgvelz: " << velavgz);
        }
        packet->Print(std::cout);
        std::cout << std::endl;
        //Schedule next broadcast 
        Simulator::Schedule (m_broadcast_time, &CustomApplication::BroadcastInformation, this);
    }

    bool CustomApplication::ReceivePacket (Ptr<NetDevice> device, Ptr<const Packet> packet,uint16_t protocol, const Address &sender)
    {
        NS_LOG_UNCOND (device << " " << packet << " " << protocol << " " << sender << " " << GetNode()->GetDevice(0)->GetAddress());
        /*
            Packets received here only have Application data, no WifiMacHeader. 
            We created packets with 1000 bytes payload, so we'll get 1000 bytes of payload.
        */
        NS_LOG_UNCOND ("ReceivePacket() : Node " << GetNode()->GetId() << " : Received a packet from " << sender << " Size:" << packet->GetSize());
        
        //Let's check if packet has a tag attached!
        CustomDataTag tag;
        if (packet->PeekPacketTag (tag))
        {
            NS_LOG_UNCOND ("\tFrom Node Id: " << tag.GetNodeId() << " at " << tag.GetPosition() << " moving at " << tag.GetVelocity()
                            << "\tPacket Timestamp: " << tag.GetTimestamp() << " delay="<< Now()-tag.GetTimestamp());
        }
        return true;
    }

    void CustomApplication::PromiscRx (Ptr<const Packet> packet, uint16_t channelFreq, WifiTxVector tx, MpduInfo mpdu, SignalNoiseDbm sn)
    {
        //This is a promiscous trace. It will pick broadcast packets, and packets not directed to this node's MAC address.
        /*
            Packets received here have MAC headers and payload.
            If packets are created with 1000 bytes payload, the size here is about 38 bytes larger. 
        */
        // NS_LOG_UNCOND (Now () << " PromiscRx() : Node " << GetNode()->GetId() << " : ChannelFreq: " << channelFreq << " Mode: " << tx.GetMode()
        //              << " Signal: " << sn.signal << " Noise: " << sn.noise << " Size: " << packet->GetSize()
        //              << " Mode " << tx.GetMode ()
        //              );    
        WifiMacHeader hdr;
        if (packet->PeekHeader (hdr))
        {
            //Let's see if this packet is intended to this node
            // Mac48Address destination = hdr.GetAddr1();
            // Mac48Address source = hdr.GetAddr2();

            // // UpdateNeighbor (source);

            // Mac48Address myMacAddress = m_waveDevice->GetMac(CCH)->GetAddress();
            // //A packet is intened to me if it targets my MAC address, or it's a broadcast message
            // if ( destination==Mac48Address::GetBroadcast() || destination==myMacAddress)
            // {
            //     NS_LOG_UNCOND ("PromiscRx() "<<"\tFrom: " << source << "\n\tSeq. No. " << hdr.GetSequenceNumber() );
            //     //Do something for this type of packets
            // }
            // else //Well, this packet is not intended for me
            // {
            //     //Maybe record some information about neighbors
            // }    
        }
    }
}


using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Net.Sockets;
using System.Net;
using System.Threading;

namespace InHomeServer
{
    public partial class Form1 : Form
    {
        bool SearchCompleted = false;

        public Form1()
        {
            InitializeComponent();
        }
        public static string GetLocalIPAddress()
        {
            var host = Dns.GetHostEntry(Dns.GetHostName());
            foreach (var ip in host.AddressList)
            {
                if (ip.AddressFamily == AddressFamily.InterNetwork)
                {
                    return ip.ToString();
                }
            }
            throw new Exception("No network adapters with an IPv4 address in the system!");
        }
        public void DisplayLocalHostName()
        {
            try
            {
                // Get the local computer host name.
                String hostName = Dns.GetHostName();
                Console.WriteLine("Computer name :" + hostName);
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException caught!!!");
                Console.WriteLine("Source : " + e.Source);
                Console.WriteLine("Message : " + e.Message);
            }
            catch (Exception e)
            {
                Console.WriteLine("Exception caught!!!");
                Console.WriteLine("Source : " + e.Source);
                Console.WriteLine("Message : " + e.Message);
            }
        }
        bool Check(string ip,string host,int port,int timeout)
        {
            try
            {
                var client = new TcpClient();
                var result = client.BeginConnect(ip, port, null, null);

                var success = result.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(timeout));

                if (!success)
                {
                    return false;
                }

                // we have connected
                client.EndConnect(result);
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
            //using (TcpClient tcpClient = new TcpClient())
            //{

            //    try
            //    {
             
            //        tcpClient.Connect(ip, port);
            //        return true;
            //       // Console.WriteLine("Port open");
            //    }
            //    catch (Exception)
            //    {
            //     //   Console.WriteLine("Port closed");
            //        return false;
            //    }
            //}
        
        }
        void GetAvailableDevices(int start, int end, int port, int timeout,ref List<string> avaliableDevices)
        {
         //  List<string> devices=new List<string>();

           string IPAddress = GetLocalIPAddress();
           string[] sp = IPAddress.Split('.');
           string host = sp[0] + "." + sp[1] + "." + sp[2]+".";
           for (int i = start; i < end; i++)
           {
               string tempIP = host + i.ToString();
               Console.WriteLine(tempIP);
               if (Check(tempIP, host, port, timeout))
               {
                   avaliableDevices.Add(tempIP);
                  
               }
       
           }

          

          //  return devices;
        
        }
        private void button1_Click(object sender, EventArgs e)
        {
    
            Thread[] myNewThread=new Thread[18]; 
            List<string> avaliableDevices = new List<string>();
            for (int i = 0; i < 17; i++)
            {
                Console.WriteLine((i * 15 + 1).ToString() + "to " + (i * 15 + 15).ToString());
          
                 myNewThread[i] = new Thread(() => GetAvailableDevices(i * 15 + 1, i * 15 +15, 1217, 1, ref avaliableDevices));
                myNewThread[i].Start();

            }
            for (int i = 1; i < 17; i++)
            {
       
                myNewThread[i].Join();
            }
          



           Console.WriteLine("def");

 
        
        }
    }
}

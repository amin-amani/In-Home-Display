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
using System.Threading.Tasks;

namespace InHomeServer
{
    public partial class Form1 : Form
    {
        
        List<string> AvaliableDevices = new List<string>();
        List<ManualResetEvent> events = new List<ManualResetEvent>();
        public static event EventHandler OperationCompleted;
        int MaxDevices = 1;
        string Host;


        public Form1()
        {
            InitializeComponent();
            OperationCompleted += Form1_OperationCompleted;
        }

        void Form1_OperationCompleted(object sender, EventArgs e)
        {
            
            PgrProgress.Invoke(new MethodInvoker(delegate
                    {
                        PgrProgress.Value++;
                        PgrProgress.Refresh();
                    }));
      
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

         bool Check(string ip,int port,int timeout)
        {
            try
            {
                if (AvaliableDevices.Count >= MaxDevices) return false;
                var client = new TcpClient();
                var result = client.BeginConnect(ip, port, null, null);
                Console.WriteLine("check"+ip);
                var success = result.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(timeout));

                if (!success)
                {
                    Console.WriteLine("end......." + ip + " fail");
                    return false;
                }

                // we have connected
                client.EndConnect(result);
                Console.WriteLine("end......."+ip+" succsess");
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }

        
        }

        private void   TaskCallBack(Object ThreadNumber)
        {
            if (Check(Host + ((int)(ThreadNumber)).ToString(), 1217, 1))
            {
                AvaliableDevices.Add (Host + ((int)(ThreadNumber)).ToString());
                       CmbDeviceName.Invoke(new MethodInvoker(delegate
                    {
                        CmbDeviceName.Items.Add(Host + ((int)(ThreadNumber)).ToString());    
                    }));
        
                
            }
            OnOperationCompleted();
            events[(int)(ThreadNumber)-1].Set();

        }

        static void OnOperationCompleted()
        {
            if (OperationCompleted != null)
                OperationCompleted(null, EventArgs.Empty);
        }

        void StartSearch() {
        
            events.Clear();
            AvaliableDevices.Clear();
            string IPAddress = GetLocalIPAddress();
            string[] sp = IPAddress.Split('.');
            Host = sp[0] + "." + sp[1] + "." + sp[2] + ".";

            for (int i = 0; i < 255; i++)
            {
                var resetEvent = new ManualResetEvent(false);

                events.Add(resetEvent);
            }

            Parallel.For(0, 255, new ParallelOptions { MaxDegreeOfParallelism = 20 },
            task =>
            {
                try
                {
                    //doneEvents[task] = new ManualResetEvent(false);
                    //var resetEvent = new ManualResetEvent(false);
                    ThreadPool.QueueUserWorkItem(new WaitCallback(TaskCallBack), task + 1);
                }
                finally
                {

                }
                //events.Add(resetEvent);

            });

            foreach (var ev in events)
                ev.WaitOne();


            Console.WriteLine("fin.....................");
          //  CmbDeviceName.Items.AddRange(AvaliableDevices.ToArray());
            MessageBox.Show("finished");
            return;
        
        }
        private void button1_Click(object sender, EventArgs e)
        {
            PgrProgress.Value = 0;
            PgrProgress.Maximum = 255;
    Thread thrSearch = new Thread(new ThreadStart(StartSearch));
    thrSearch.Start();
        
        }
    }
}

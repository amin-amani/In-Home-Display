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
using System.Globalization;
using System.IO;

namespace InHomeServer
{
    public partial class Form1 : Form
    {
        
        List<string> AvaliableDevices = new List<string>();
        List<ManualResetEvent> events = new List<ManualResetEvent>();
        public static event EventHandler OperationCompleted;
        int MaxDevices = 1;
        string Host;
        Thread thrUpdate;
        bool busy=false;


        public Form1()
        {
            InitializeComponent();
            OperationCompleted += Form1_OperationCompleted;
        }

        void Form1_OperationCompleted(object sender, EventArgs e)
        {
            try
            {
                PgrProgress.Invoke(new MethodInvoker(delegate
                {
                    PgrProgress.Value++;
                    PgrProgress.Refresh();
                }));
            }
            catch (Exception)
            { }
            
            
        }
        public DateTime GetDeviceClock()
        {
            DateTime result = new DateTime();
            try
            {

                TCPHandler client = new TCPHandler();


                if (!client.Connect(CmbDeviceName.Text, 1217, 10))
                {
                    Console.WriteLine("end......." + CmbDeviceName.Text + " fail");
                    return result;
                }
                string resp = client.SendCommandMs("GetTime\n", 20, 10, 1);
                //DateTime.TryParseExact(resp.Trim(),
                //              "yyyy-dd-MM hh:mm:ss",
                //              CultureInfo.InvariantCulture,
                //              DateTimeStyles.None,
                //              out result);
                result = DateTime.ParseExact(resp.Trim(), "yyyy-MM-dd HH:mm:ss",
                                           System.Globalization.CultureInfo.InvariantCulture);
                //  result = new DateTime();
                //MessageBox.Show(resp);
                client.Disconnect();
                return result;                
            }
            catch(Exception){
                return result;
            }

        }

        private bool GetOnlineParameters(ref float voltage, ref  float current, ref  float energy,ref int temp)
        {
            

            TCPHandler client = new TCPHandler();


            if (!client.Connect(CmbDeviceName.Text, 1217, 10))
            {
                Console.WriteLine("end......." + CmbDeviceName.Text + " fail");
                return false;
            }
            string resp = client.SendCommandMs("Online\n", 40, 10, 1);
            string []sp=resp.Split(',');
            if (sp.Length < 4) return false;
        
            float.TryParse(sp[0], out voltage);
            float.TryParse(sp[1], out current);
            float.TryParse(sp[2], out energy);
            int.TryParse(sp[3], out temp);
 //Console.WriteLine("resp ------------------->>> "+sp[0]+" "+ sp[1]+" "+sp[2]);
            //  result = new DateTime();
            //MessageBox.Show(resp);
            client.Disconnect();
            return true;
        }



          private void ReadMem()
        { 
              TCPHandler client = new TCPHandler();
              
                  BtnDownload.Invoke(new MethodInvoker(delegate
           {
            if (!client.Connect(CmbDeviceName.Text, 1217, 10))
            {
                Console.WriteLine("end......." + CmbDeviceName.Text + " fail");
            
                    BtnDownload.Enabled = true;
                    TimUpdateValues.Enabled = true;
              
                return;
         
            }  
                
                }));
            //string resp = 
              StreamWriter sw = new StreamWriter("export.txt");
            for (int i = 0; i < 336; i++) { 
            string resp = client.SendCommandReadLine("ReadRecord "+i.ToString()+"\n", 40, 10, 1);
            PgrProgress.Invoke(new MethodInvoker(delegate
            {

                PgrProgress.Value++; ;
            }));
           // Thread.Sleep(1);
            if (!resp.Contains("error")) {  sw.WriteLine(resp); }
            Console.WriteLine(resp);
            }
            sw.Close();
            BtnDownload.Invoke(new MethodInvoker(delegate
    {
        BtnDownload.Enabled = true;
        TimUpdateValues.Enabled = true;
    }));

            //List<byte> resp = client.SendCommandMs(Encoding.ASCII.GetBytes("ReadRecord\n").ToList(), 13, 200, 1);
              //MessageBox.Show("Test");
           // MessageBox.Show(resp);
        }
        public DateTime SetDeviceClock()
        {
            DateTime result =DateTime.Now;

            TCPHandler client = new TCPHandler();
            string msg = "SetTime " + (result.Year-2000).ToString() + " " + result.Month.ToString() + " "
    + result.Day.ToString() + " 1 " + result.Hour.ToString() + " " + result.Minute.ToString() + " " +
    result.Second.ToString() + "\n";
            MessageBox.Show(msg);

            if (!client.Connect(CmbDeviceName.Text, 1217, 5))
            {
                Console.WriteLine("end......." + CmbDeviceName.Text + " fail");
                return result;
            }

            string resp = client.SendCommandMs(msg, 40, 10, 1);

   
            //  result = new DateTime();
            MessageBox.Show(resp);
            client.Disconnect();
            return result;
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
                client.Close();
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
           // MessageBox.Show("finished");
            return;
        
        }
        private void button1_Click(object sender, EventArgs e)
        {
            PgrProgress.Value = 0;
            PgrProgress.Maximum = 255;
    Thread thrSearch = new Thread(new ThreadStart(StartSearch));
    thrSearch.IsBackground = true;
    thrSearch.Start();
        
        }

      
        private void button1_Click_1(object sender, EventArgs e)
        {
            
            TimUpdateValues.Enabled = false;
            if (thrUpdate!=null)
            while (thrUpdate.IsAlive) ;   
            MessageBox.Show(   GetDeviceClock().ToString());
            TimUpdateValues.Enabled = true;
        }

        private void button2_Click(object sender, EventArgs e)
        {
            TimUpdateValues.Enabled = false;
            if (thrUpdate != null)
                while (thrUpdate.IsAlive) ;
            SetDeviceClock(); 
            //MessageBox.Show(GetDeviceClock().ToString());
            TimUpdateValues.Enabled = true;
            //ReadMem();
            //SetDeviceClock(); 

        }

        private void TimUpdateValues_Tick(object sender, EventArgs e)
        {

            Thread thrUpdate = new Thread(new ThreadStart(StartUpdate));
            thrUpdate.IsBackground = true;
            thrUpdate.Start();
        }
        void StartUpdate()
        {

            float v=0, i=0, e=0;
            int temp=0;
            LabDeviceDate.Invoke(new MethodInvoker(delegate
           {
               LabDeviceDate.Text = GetDeviceClock().ToString();
               if (GetOnlineParameters(ref v, ref i, ref e,ref temp))
               {
                   LabVoltage.Text ="Voltage="+ v.ToString()+" V";
                   LabCurrent.Text = "Current=" + i.ToString()+" A";
                   LabEnergy.Text = "Energy=" + e.ToString()+" Kwh";
                   LabTemp.Text = "Temp=" + temp.ToString() + " C";
               }
               

           }));

          
     
        }
        private void Form1_Load(object sender, EventArgs e)
        {
    
        }

        private void BtnDownload_Click(object sender, EventArgs e)
        {
            TimUpdateValues.Enabled = false;
            if (thrUpdate != null)
                while (thrUpdate.IsAlive) ;
            //ReadMem();
            ////MessageBox.Show(GetDeviceClock().ToString());
            //TimUpdateValues.Enabled = true;




            PgrProgress.Value = 0;
            PgrProgress.Maximum = 336;
            Thread thrReadmem = new Thread(new ThreadStart(ReadMem));
            thrReadmem.IsBackground = true;
            thrReadmem.Start();
            BtnDownload.Enabled = false;
            
        }
    }
}

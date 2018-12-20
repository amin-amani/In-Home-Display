using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Net.Sockets;
using System.Threading;

namespace InHomeServer
{
    class TCPHandler
    {
        Int32 _port;
        string _server;
        TcpClient client=null;
        NetworkStream stream;
        public void Disconnect() 
        {
            try
            {
                client.Close();
            }
            catch (Exception ex)
            {
                Console.WriteLine(ex.Message);
            }
        }
        public bool Connect(String server, Int32 port,int timeout)
        {
             client = new TcpClient();
            _server = server;
            client.ReceiveBufferSize = 1;
            client.SendBufferSize = 1;
            _port = port;
            var result = client.BeginConnect(_server, _port, null, null);

            var success = result.AsyncWaitHandle.WaitOne(TimeSpan.FromSeconds(timeout));

            if (!success)
            {
                //throw new Exception("Failed to connect.");
                Console.WriteLine("connectio failed!");
                return false;
            }

            // we have connected
             stream = client.GetStream();
         //   client.EndConnect(result);
            return true;
        }
        public void Write(List<byte>data)
        {
            NetworkStream stream = client.GetStream();
            stream.Write(data.ToArray(), 0, data.Count);
        }
        public List<byte> SendCommand(List<byte> data,int timeout,int writeDelay)
        {
            try

            {
                List<byte> result = new List<byte>();
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                 // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream();
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);
                
                }
                    NetworkStream stream = client.GetStream();

                // Send the message to the connected TcpServer. 
               // stream.Write(data.ToArray(), 0, data.Count);
                for (int j = 0; j < data.Count;j++ )
                {
                    stream.Write(data.ToArray(), j, 1);
                    Thread.Sleep(writeDelay);
                    stream.Flush();
                    
                }

                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout*100;
                // Receive the TcpServer.response.
                using (stream = client.GetStream())
                {

                    try
                    {

                        var bytesRead = stream.Read(receiveBuffer, 0, (int)client.ReceiveBufferSize);
                        if (bytesRead == 0)
                        {
                            return result;
                        }


                    }
                    catch (Exception )
                    {
                        return result;
                    
                    }

                }
                // Buffer to store the response bytes.
                
                
                // Read the first batch of the TcpServer response bytes.



                return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }

        
        }
        public List<byte> Read( int timeout)
        {
            try
            {
                List<byte> result = new List<byte>();
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream();
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);

                }
                NetworkStream stream = client.GetStream();

                // Send the message to the connected TcpServer. 
               
                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout * 1000;
                // Receive the TcpServer.response.
                using (stream = client.GetStream())
                {

                    try
                    {

                        var bytesRead = stream.Read(receiveBuffer, 0, (int)client.ReceiveBufferSize);
                        if (bytesRead == 0)
                        {
                            return result;
                        }


                    }
                    catch (Exception)
                    {
                        return result;

                    }

                }
                // Buffer to store the response bytes.


                // Read the first batch of the TcpServer response bytes.



                return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }


        }
        public List<byte> SendCommand(List<byte> data,int expected, int timeout,int writeDelay)
        {
            try
            {
                List<byte> result = new List<byte>();
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream(); 
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);
                     stream = client.GetStream();
                }
                
                Console.WriteLine("tcp connected=====================>"+client.Connected.ToString());
                if (!client.Connected) { client.Close(); return result; }
                // Send the message to the connected TcpServer. 

                stream.Write(data.ToArray(), 0, data.Count);
                stream.Flush();
             //   for (int j = 0; j < data.Count; j++)
             //   {
             //       stream.Write(data.ToArray(), j, 1);
            //       Thread.Sleep(writeDelay);
             //       stream.Flush();

              //  }
                
                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout * 1000;
                // Receive the TcpServer.response.
              //  using (stream = client.GetStream())
                //{
                    while(true){
                    try
                    {


                        var bytesRead = stream.Read(receiveBuffer, 0, client.ReceiveBufferSize);

                        //if (bytesRead == 0)
                        //{
                        //    return result;
                        //}
                        for (int i = 0; i < bytesRead; i++)
                        {
                            result.Add(receiveBuffer[i]);
                        }
                        if (result.Count >= expected) { client.Close(); return result; }

                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("catch ex: {0}", ex);
                        client.Close();
                        return result;

                    }
                }//while

              //  }
                // Buffer to store the response bytes.


                // Read the first batch of the TcpServer response bytes.



               // return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }


        }
        public List<byte> SendCommandMs(List<byte> data, int expected, int timeout, int writeDelay)
        {
            try
            {
                List<byte> result = new List<byte>();
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream(); 
                client.ReceiveBufferSize = 10000;
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);
                    stream = client.GetStream();
                }
                Console.WriteLine("tcp connected=====================>" + client.Connected.ToString());
                if (!client.Connected || !stream.CanWrite) { return result; }
                
                // Send the message to the connected TcpServer. 
                try
                {
                    stream.Write(data.ToArray(), 0, data.Count);
                    stream.Flush();
                }
                catch(Exception){} 
                //   for (int j = 0; j < data.Count; j++)
                //   {
                //       stream.Write(data.ToArray(), j, 1);
                //       Thread.Sleep(writeDelay);
                //       stream.Flush();

                //  }

                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout;
                // Receive the TcpServer.response.
                //  using (stream = client.GetStream())
                //{
                while (true)
                {
                    try
                    {


                        var bytesRead = stream.Read(receiveBuffer, 0, client.ReceiveBufferSize);

                        //if (bytesRead == 0)
                        //{
                        //    return result;
                        //}
                        for (int i = 0; i < bytesRead; i++)
                        {
                            result.Add(receiveBuffer[i]);
                        }
                        if (result.Count >= expected) { client.Close(); return result; }

                    }
                    catch (Exception ex)
                    {
                        client.Close();
                        Console.WriteLine("catch ex: {0}", ex);
                        return result;

                    }
                }//while

                //  }
                // Buffer to store the response bytes.


                // Read the first batch of the TcpServer response bytes.



                // return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }


        }
        ////
        public string SendCommandReadLine(String data, int expected, int timeout, int writeDelay)
        {
            try
            {
                String result = "";
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream(); 
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);
                    stream = client.GetStream();
                }
                //Console.WriteLine("tcp connected=====================>" + client.Connected.ToString());
                if (!client.Connected || !stream.CanWrite) { return result; }

                // Send the message to the connected TcpServer. 
                try
                {
                    stream.Write(Encoding.ASCII.GetBytes(data), 0, data.Length);
                    stream.Flush();
                }

                catch (Exception) { }
                client.ReceiveBufferSize = expected;
                //  Thread.Sleep(100);
                //   for (int j = 0; j < data.Count; j++)
                //   {
                //       stream.Write(data.ToArray(), j, 1);
                //       Thread.Sleep(writeDelay);
                //       stream.Flush();

                //  }

                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout;
                // Receive the TcpServer.response.
                //  using (stream = client.GetStream())
                //{
                while (true)
                {
                    try
                    {


                        var bytesRead = stream.Read(receiveBuffer, 0, client.ReceiveBufferSize);

                        //if (bytesRead == 0)
                        //{
                        //    return result;
                        //}
                        for (int i = 0; i < bytesRead; i++)
                        {
                            //result.Add(receiveBuffer[i]);
                            result += (char)receiveBuffer[i];
                            if (receiveBuffer[i] == '\n')
                            { client.Close(); return result; }
                        }

                        if (result.Length >= expected)
                        {
                            client.Close(); return result;
                        }
                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("catch ex: {0}", ex);
                        client.Close();
                        return result;

                    }
                }//while

                //  }
                // Buffer to store the response bytes.


                // Read the first batch of the TcpServer response bytes.



                // return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }


        }
    
    
        ////
        public string SendCommandMs(String data, int expected, int timeout, int writeDelay)
        {
            try
            {
                String result = "";
                // Create a TcpClient.
                // Note, for this client to work you need to have a TcpServer 
                // connected to the same address as specified by the server, port
                // combination.
                // Get a client stream for reading and writing.
                //  Stream stream = client.GetStream(); 
                if (!client.Connected)
                {
                    client = new TcpClient();
                    client.Connect(_server, _port);
                     stream = client.GetStream();
                }
                Console.WriteLine("tcp connected=====================>" + client.Connected.ToString());
                if (!client.Connected || !stream.CanWrite) { return result; }

                // Send the message to the connected TcpServer. 
                try
                {
                    stream.Write(Encoding.ASCII.GetBytes(data), 0, data.Length);
                    stream.Flush();
                }

                catch (Exception) { }
                client.ReceiveBufferSize = expected;
              //  Thread.Sleep(100);
                //   for (int j = 0; j < data.Count; j++)
                //   {
                //       stream.Write(data.ToArray(), j, 1);
                //       Thread.Sleep(writeDelay);
                //       stream.Flush();

                //  }

                byte[] receiveBuffer = new Byte[client.ReceiveBufferSize];
                stream.ReadTimeout = timeout;
                // Receive the TcpServer.response.
                //  using (stream = client.GetStream())
                //{
                while (true)
                {
                    try
                    {


                        var bytesRead = stream.Read(receiveBuffer, 0, client.ReceiveBufferSize);

                        //if (bytesRead == 0)
                        //{
                        //    return result;
                        //}
                        for (int i = 0; i < bytesRead; i++)
                        {
                            //result.Add(receiveBuffer[i]);
                            result += (char)receiveBuffer[i];
                        }
                        if (result.Length >= expected) return result;

                    }
                    catch (Exception ex)
                    {
                        Console.WriteLine("catch ex: {0}", ex);
                        return result;

                    }
                }//while

                //  }
                // Buffer to store the response bytes.


                // Read the first batch of the TcpServer response bytes.



                // return receiveBuffer.ToList<byte>();
            }
            catch (ArgumentNullException e)
            {
                Console.WriteLine("ArgumentNullException: {0}", e);
                return null;
            }
            catch (SocketException e)
            {
                Console.WriteLine("SocketException: {0}", e);
                return null;
            }


        }
    
    
    
    }
}

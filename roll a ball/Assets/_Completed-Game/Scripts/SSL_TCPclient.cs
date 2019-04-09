 using UnityEngine;
 using System;
 using System.Collections;
 using System.Net.Security;
 using System.Net.Sockets;
 using System.Security.Authentication;
 using System.Text;
 using System.Security.Cryptography.X509Certificates;
 using System.Runtime.InteropServices;
 
 public class SSL_TCPclient : MonoBehaviour
{
     public const int BUFFSIZE1 = 32000;
 
     [StructLayout(LayoutKind.Sequential, Pack = 4)]
     public struct cheader
     {
         ushort tran_type;
         public ushort data_size;
         uint user_id;
         byte iscontinue;
         ushort branch_id;
         [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = 4)]
         byte[] tran_date;
         int encrypt;
     };
 
     [StructLayout(LayoutKind.Sequential, Pack = 4)]
     public struct sheader
     {
         public cheader chdr;
         int srv_trno;
         byte iscontinue;
         byte send_count;
         int result_nsec;
     };
 
     public struct client_buffer
     {
         cheader chdr;
         [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = BUFFSIZE1)]
         byte[] buffer;
     };
 
     public struct server_buffer
     {
         sheader shdr;
         [MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.U4, SizeConst = BUFFSIZE1)]
         byte[] buffer;
     };
 
     public struct tran_time
     {
         long res_nsec;
         long res_usec;
         long req_nsec;
         long req_usec;
     };
 
     private static Hashtable certificateErrors = new Hashtable();
 
     X509Certificate clientCertificate;
 
     public static string serverMessage;
 
     public static int counter = 0;
 
     private string fileName;
 
     public static bool CertificateValidationCallback(
             object sender,
             X509Certificate certificate,
             X509Chain chain,
             SslPolicyErrors sslPolicyErrors)
     {
         return true;
     }
 
     static X509Certificate CertificateSelectionCallback(object sender,
     string targetHost,
     X509CertificateCollection localCertificates,
     X509Certificate remoteCertificate,
     string[] acceptableIssuers)
     {
         return localCertificates[0];
     }
 
     public void RunClient(string machineName)
     {
         fileName = "1411134668.p12";
 
         TcpClient client = new TcpClient();
         client.Connect(machineName, 4433);
 
         counter++;
 
         SslStream sslStream = new SslStream(client.GetStream(), false,
             new RemoteCertificateValidationCallback(CertificateValidationCallback),
             new LocalCertificateSelectionCallback(CertificateSelectionCallback));
 
         bool authenticationPassed = true;
         try
         {
             string path = getPath();
 
 #if UNITY_EDITOR
             X509Certificate2 cert = new X509Certificate2(path, "test");
 #elif UNITY_ANDROID
             WWW reader = new WWW(path);
             while (!reader.isDone) { }
             X509Certificate2 cert = new X509Certificate2(reader.bytes, "test");
 #endif
             X509Certificate2Collection certs = new X509Certificate2Collection();
             certs.Add(cert);
 
             counter++;
 
             sslStream.AuthenticateAsClient(
                 machineName,
                 certs,
                 SslProtocols.Tls,
                 true); // check cert revokation
 
             counter++;
         }
         catch (AuthenticationException e)
         {
             Debug.Log("Exception: " + e.Message);
             if (e.InnerException != null)
             {
                 Debug.Log("Inner exception: " + e.InnerException.Message);
             }
             Debug.Log("Authentication failed - closing the connection.");
             client.Close();
             authenticationPassed = false;
             return;
         }
 
         if (authenticationPassed)
         {
             byte[] messsage = Encoding.UTF8.GetBytes("Hello from the client.<EOF>");
 
             Debug.Log("After : " + sslStream.LocalCertificate);
 
 
             serverMessage = ReadMessage(sslStream);
 
             Debug.Log("Server says: " + serverMessage);
 
             client.Close();
             Debug.Log("Client closed.");
 
             counter++;
 
         }
     }
 
     private string getPath()
     {
 #if UNITY_EDITOR
         return Application.streamingAssetsPath + "/" + fileName;
 #elif UNITY_ANDROID
         return Application.streamingAssetsPath + "/" + fileName;
 #elif UNITY_IPHONE
         return GetiPhoneDocumentsPath()+"/" + fileName;
 #else
         return Application.dataPath +"/" + fileName;
 #endif
     }
 
     static string ReadMessage(SslStream sslStream)
     {
 
         byte[] buffer = new byte[2048];
         StringBuilder messageData = new StringBuilder();
 
         int k = sslStream.Read(buffer, 0, 200);
 
         Debug.Log("Value of k : " + k);
 
         sheader returndata = ByteArrayToStructure<sheader>(buffer);
 
         Debug.Log("This is what the host returned to you: " + returndata);
 
         Debug.Log("Buffer Size : " + k);
 
         char[] c = new char[k];
 
         for (int i = 0; i < k; i++)
         {
             
             c[i] = Convert.ToChar(buffer[i]);
         }
 
         string s = new string(c);
 
         return s;
     }
 
     static sheader ByteArrayToStructure<sheader>(byte[] bytes)
     {
         GCHandle handle = GCHandle.Alloc(bytes, GCHandleType.Pinned);
         sheader stuff = (sheader)Marshal.PtrToStructure(handle.AddrOfPinnedObject(), typeof(sheader));
         handle.Free();
         return stuff;
     }
 
     public static void callFunc(string addr)
     {
         SSL_TCPclient obj = new SSL_TCPclient();
         Environment.SetEnvironmentVariable("MONO_TLS_SESSION_CACHE_TIMEOUT", "0");
         obj.RunClient(addr);
     }
 
     private static void DisplayUsage()
     {
         Debug.Log("To start the client specify:");
         Debug.Log("clientSync machineName [serverName]");
         Environment.Exit(1);
     }
 
 }
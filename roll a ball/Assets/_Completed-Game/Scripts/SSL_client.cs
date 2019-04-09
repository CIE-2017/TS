using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;

public class SSL_client : MonoBehaviour
{
    static string server = "192.168.153.130";
    static TcpClient client = new TcpClient(server, 4433);
    SslStream sslStream = new SslStream(client.GetStream(), false, new RemoteCertificateValidationCallback(ValidateServerCertificate), null);
    // Start is called before the first frame update
    void Start()
    {        
        sslStream.AuthenticateAsClient(server);
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        byte[] messsage = Encoding.UTF8.GetBytes("Hello from the client.");
        sslStream.Write(messsage);
        string answer = ReadMessage(sslStream);
        Debug.Log("Server says: " + answer);
    }   

    void OnApplicationQuit()
    {
        client.Close();
    }

    //Validate the server ceretificate
    public static bool ValidateServerCertificate(object sender, X509Certificate certificate,
    X509Chain chain, SslPolicyErrors sslPolicyErrors)
    {
        return true;
    }

    //read the message sent from sever
    string ReadMessage(SslStream sslStream)
    {
        byte[] buffer = new byte[2048];
        StringBuilder messageData = new StringBuilder();
        int bytes = -1;
        bytes = sslStream.Read(buffer, 0, buffer.Length);
        Decoder decoder = Encoding.UTF8.GetDecoder();
        char[] chars = new char[decoder.GetCharCount(buffer, 0, bytes)];
        decoder.GetChars(buffer, 0, bytes, chars, 0);
        messageData.Append(chars);
        Debug.Log(messageData.ToString());
        return messageData.ToString();
    }
}

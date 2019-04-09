using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;

public class net_test : MonoBehaviour
{
    // Start is called before the first frame update
    void Start()
    {
        string server = "192.168.153.130";
        TcpClient client = new TcpClient(server, 4433);

        using (SslStream sslStream = new SslStream(client.GetStream(), false,
            new RemoteCertificateValidationCallback(ValidateServerCertificate), null))
        {
            sslStream.AuthenticateAsClient(server);
            // This is where you read and send data
            byte[] messsage = Encoding.UTF8.GetBytes("Hello from the client.");
            sslStream.Write(messsage);
            string answer = ReadMessage(sslStream);
            Debug.Log("Server says: " + answer);
        }
        client.Close();
    }

    // Update is called once per frame
    void Update()
    {
        
    }

    public static bool ValidateServerCertificate(object sender, X509Certificate certificate,
    X509Chain chain, SslPolicyErrors sslPolicyErrors)
    {
        return true;
    }

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

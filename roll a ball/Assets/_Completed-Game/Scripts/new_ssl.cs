using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Sockets;
using System.Net.Security;
using System.Text;

public class new_ssl : MonoBehaviour
{
    TcpClient mail = new TcpClient();
    SslStream sslStream;
    int bytes = -1;
    byte[] buffer = new byte[2048];
    // Start is called before the first frame update
    void Start()
    {


        mail.Connect("192.168.153.130", 4433);
        sslStream = new SslStream(mail.GetStream());

        sslStream.AuthenticateAsClient("192.168.153.130");

        // Read the stream to make sure we are connected
        bytes = sslStream.Read(buffer, 0, buffer.Length);
        Debug.Log(Encoding.ASCII.GetString(buffer, 0, bytes));

    }

    // Update is called once per frame
    void Update()
    {
        //Send the users login details
        sslStream.Write(Encoding.ASCII.GetBytes("USER USER_EMAIL\r\n"));
        bytes = sslStream.Read(buffer, 0, buffer.Length);
        Debug.Log(Encoding.ASCII.GetString(buffer, 0, bytes));
    }
}



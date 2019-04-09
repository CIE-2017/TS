using System.Net.Security;
using System.Net.Sockets;
using UnityEngine;
public class TlsTest : MonoBehaviour
{
    void Start()
    {
        TcpClient client = new TcpClient();
        client.Connect("192.168.153.130", 4433);
        SslStream ssl = new SslStream(client.GetStream());
        ssl.AuthenticateAsClient("192.168.153.130");
    }
}
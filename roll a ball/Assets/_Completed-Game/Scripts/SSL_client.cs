using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System.Net.Security;
using System.Net.Sockets;
using System.Security.Cryptography.X509Certificates;
using System.Text;
using System.Threading;


public class Account
{
    public string Id;
    public string Horizontal;
    public string Vertical;
}

public class SSL_client : MonoBehaviour
{
    public Account json_obj;
    private Thread _t1;
    public string answer;
    static string server = "172.30.154.38";
    static TcpClient client = new TcpClient(server, 4433);
    SslStream sslStream = new SslStream(client.GetStream(), false, new RemoteCertificateValidationCallback(ValidateServerCertificate), null);
    // Start is called before the first frame update

     void _func1()
    {
        while (true)
        {
            try
            {
                // sslStream.ReadTimeout = 500;

                answer = ReadMessage(sslStream);
                json_obj = JsonUtility.FromJson<Account>(answer);
                //Debug.Log(json_obj.Id);
            }
            catch
            {
                //Debug.Log("dfs")
                json_obj = null;
            }
        }
    }

    void Start()
    {        
        sslStream.AuthenticateAsClient(server);
        _t1 = new Thread(_func1);
        _t1.Start();
    }

    // Update is called once per frame
    void FixedUpdate()
    {
        //Test slot1 = new Test(123, 123 );
        float Horizontal = Input.GetAxis("Horizontal");
        float Vertical = Input.GetAxis("Vertical");
        Account user = new Account();
        if (Horizontal != 0.0f || Vertical != 0.0f)
        {
            user.Horizontal = Horizontal.ToString();
            user.Vertical = Vertical.ToString();
            user.Id = "1";
            string result = JsonUtility.ToJson(user);
            byte[] send = Encoding.ASCII.GetBytes(result);
            try
            {
                sslStream.Write(send, 0, send.Length);
            }
            catch {
                Debug.Log("lol");
            }
        }
        /*
        if (answer != null) {
            json_obj = JsonUtility.FromJson<Account>(answer);
        }
        else
        {
            json_obj = JsonUtility.FromJson<Account>("{}");
        }*/

        //Debug.Log(answer);
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
        //Debug.Log(messageData.ToString());
        return messageData.ToString();
    }
}

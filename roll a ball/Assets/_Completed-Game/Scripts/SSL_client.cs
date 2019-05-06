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
    public string RoomId;
    public string Id;
    public string Horizontal;
    public string Vertical;
    public string pos_x;
    public string pos_z;
}

public class SSL_client : MonoBehaviour
{
    public Account json_obj;
    private Thread _t1;
    public string answer;
    static string server = "172.30.154.39";
    
    //for google cloud
    //static string server = "35.240.210.226"; 
    public string id;
    public string room_id;
    public Vector3 position;
    static TcpClient client = new TcpClient(server, 4433);
    SslStream sslStream = new SslStream(client.GetStream(), false, new RemoteCertificateValidationCallback(ValidateServerCertificate), null);
    // Start is called before the first frame update
    public string position_x;
    public string position_z;
    string temp_x;
    string temp_z;

     void _func1()
    {
        room_id = ReadMessage(sslStream);
        id = PlayerStats.Id.ToString();
        float pos_x = float.Parse(ReadMessage(sslStream));
        float pos_z = float.Parse(ReadMessage(sslStream));
        position_x = pos_x.ToString();
        position_z = pos_z.ToString();
        position = new Vector3(pos_x, 5f, pos_z);
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

    private void Awake()
    {
        sslStream.AuthenticateAsClient(server);
        _t1 = new Thread(_func1);
        _t1.Start();
    }
    void Start()
    {        

    }

    // Update is called once per frame
    void FixedUpdate()
    {
        //Test slot1 = new Test(123, 123 );
        float Horizontal = Input.GetAxis("Horizontal");
        float Vertical = Input.GetAxis("Vertical");
        GameObject client = GameObject.Find("Player");
        PlayerController playerScript = client.GetComponent<PlayerController>();
        string pos_x = playerScript.transform.position.x.ToString();
        string pos_z = playerScript.transform.position.z.ToString();
        Account user = new Account();
        if (Horizontal != 0.0f || Vertical != 0.0f)
        {
            user.RoomId = room_id;
            user.Horizontal = Horizontal.ToString();
            user.Vertical = Vertical.ToString();
            user.Id = id;
            user.pos_x = pos_x;
            user.pos_z = pos_z;
            /*
            if (position_x != temp_x || position_z != temp_z)
            {
                user.pos_x = position_x;
            user.pos_z = position_z;
            temp_x = position_x;
            temp_z = position_z;
                }*/
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
        string close = "exit";
        byte[] bclose = Encoding.ASCII.GetBytes(close);
        sslStream.Write(bclose, 0, bclose.Length);
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
        Decoder decoder = Encoding.ASCII.GetDecoder();
        char[] chars = new char[decoder.GetCharCount(buffer, 0, bytes)];
        decoder.GetChars(buffer, 0, bytes, chars, 0);
        messageData.Append(chars);
        //Debug.Log(messageData.ToString());
        return messageData.ToString();
    }
}

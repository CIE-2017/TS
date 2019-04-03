using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using System;
using System.Net;
using System.Text;
using System.Net.Sockets;

public class tcp_client : MonoBehaviour
{
    private Socket _clientSocket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
    private byte[] _recieveBuffer = new byte[8142];

    void Start()
    {
        SetupServer();
    }

    void Update()
    {
        byte[] fileStream = new byte[2];
        fileStream[0] = 0x00C9;
        float Horizontal = Input.GetAxis("Horizontal");
        float Vertical = Input.GetAxis("Vertical");
        byte[] moveHorizontal = Encoding.ASCII.GetBytes("Horizon: " + Horizontal.ToString());
        byte[] moveVertical = Encoding.ASCII.GetBytes("Vertical: " + Horizontal.ToString());
        SendData(moveHorizontal);
        SendData(moveVertical);
    }

    void OnApplicationQuit()
    {
        _clientSocket.Close();
    }

    private void SetupServer()
    {
        try
        {
            IPAddress server = IPAddress.Parse("192.168.153.130");
            _clientSocket.Connect(new IPEndPoint(server, 5000));
        }
        catch (SocketException ex)
        {
            Debug.Log(ex.Message);
        }

        _clientSocket.BeginReceive(_recieveBuffer, 0, _recieveBuffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), null);

    }


    private void ReceiveCallback(IAsyncResult AR)
    {
        //Check how much bytes are recieved and call EndRecieve to finalize handshake
        int recieved = _clientSocket.EndReceive(AR);

        if (recieved <= 0)
            return;

        //Copy the recieved data into new buffer , to avoid null bytes
        byte[] recData = new byte[recieved];
        Buffer.BlockCopy(_recieveBuffer, 0, recData, 0, recieved);

        //Process data here the way you want , all your bytes will be stored in recData
        Debug.Log(System.Text.Encoding.Default.GetString(_recieveBuffer));
        //SendData(System.Text.Encoding.Default.GetBytes("ping"));

        //Start receiving again
        _clientSocket.BeginReceive(_recieveBuffer, 0, _recieveBuffer.Length, SocketFlags.None, new AsyncCallback(ReceiveCallback), null);

    }

    private void SendData(byte[] data)
    {
        SocketAsyncEventArgs socketAsyncData = new SocketAsyncEventArgs();
        socketAsyncData.SetBuffer(data, 0, data.Length);
        _clientSocket.SendAsync(socketAsyncData);
    }
}

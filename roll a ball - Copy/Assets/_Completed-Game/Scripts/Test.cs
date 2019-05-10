using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Test : MonoBehaviour
{
    public float speed;
    private Rigidbody rb;
    private GameObject opponents;
    // Start is called before the first frame update
    void Start()
    {
        opponents = GameObject.Find("/Opponent/Sphere");
        rb = opponents.GetComponent<Rigidbody>();
        //eiei.SetActive(false);
    }
    void FixedUpdate()
    {
        GameObject client = GameObject.Find("SSL");
        SSL_client clientScript = client.GetComponent<SSL_client>();
        string myId = clientScript.id;
        string roomId = clientScript.room_id; 
        Account json = clientScript.json_obj;
        if (json != null)
        {
            if (roomId == json.RoomId)
            {
                if (json.Id != myId)
                {
                    // Set some local float variables equal to the value of our Horizontal and Vertical Inputs
                    float moveHorizontal = float.Parse(json.Horizontal);
                    float moveVertical = float.Parse(json.Vertical);
                    float pos_x = float.Parse(json.pos_x);
                    float pos_y = float.Parse(json.pos_y);
                    float pos_z = float.Parse(json.pos_z);
                    Vector3 position = new Vector3(pos_x, pos_y, pos_z);
                    // Create a Vector3 variable, and assign X and Z to feature our horizontal and vertical float variables above
                    //Vector3 movement = new Vector3(moveHorizontal, 0.0f, moveVertical);
                    transform.position = position;
                }
            }
        }
    }
}

import sys
import paho.mqtt.client as mqtt
server = "54.82.121.229" 

client = mqtt.Client()
client.connect(server, 1883, 60)

if len(sys.argv) <= 1:
    print("Usage : "+sys.argv[0]+" on/off")
    exit()
else:
    client.publish("id/jihoon/switch/evt", str(sys.argv[1]))

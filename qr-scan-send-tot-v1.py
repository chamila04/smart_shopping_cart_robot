import cv2
import numpy as np
import pyzbar.pyzbar as pyzbar
import urllib.request
import socket

font = cv2.FONT_HERSHEY_PLAIN

url = 'http://192.168.22.219/'  #ip address of esp32 cam
cv2.namedWindow("live transmission", cv2.WINDOW_AUTOSIZE)
ESP32_IP = '192.168.22.221' #ip address of esp32 board
PORT = 80


# send values to esp32
def send_message(message):
    try:
        # Create a socket object
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        
        # Connect to ESP32
        client_socket.connect((ESP32_IP, PORT))
        print("Connected to ESP32")
        
        # Send message to ESP32
        client_socket.sendall((message + '\n').encode('utf-8'))
        print(f"Sent: {message}")
        
        # Receive acknowledgment from ESP32
        response = client_socket.recv(1024).decode('utf-8')
        print(f"ESP32 Response: {response}")
        
    except Exception as e:
        print(f"Error: {e}")
    finally:
        # Close the socket
        client_socket.close()


prev = ""
pres = ""
item_value=""
inventory = [
                ["ABC", 200],
                ["ACB", 300],
                ["BAC", 450],
                ["BCA", 350],
                ["CAB", 550],
                ["CBA", 600]
            ]
items = [
            "ABC00123", "ABC00132", "ABC00213", "ABC00231", "ABC00312", "ABC00321",
            "ACB00123", "ACB00132", "ACB00213", "ACB00231", "ACB00312", "ACB00321",
            "BAC00123", "BAC00132", "BAC00213", "BAC00231", "BAC00312", "BAC00321",
            "BCA00123", "BCA00132", "BCA00213", "BCA00231", "BCA00312", "BCA00321",
            "CAB00123", "CAB00132", "CAB00213", "CAB00231", "CAB00312", "CAB00321",
            "CBA00123", "CBA00132", "CBA00213", "CBA00231", "CBA00312", "CBA00321"
        ]            
bill = []
tot = 0
st_tot=""


while True:
    try:
        # Try to retrieve the image from the URL
        img_resp = urllib.request.urlopen(url + 'cam-hi.jpg', timeout=20)
        imgnp = np.array(bytearray(img_resp.read()), dtype=np.uint8)
        frame = cv2.imdecode(imgnp, -1)
        
        # If frame is empty or None, skip processing
        if frame is None:
            print("Failed to retrieve the image. Skipping frame.")
            continue

        decodedObjects = pyzbar.decode(frame)
        for obj in decodedObjects:
            pres = obj.data
            item_value = obj.data.decode("utf-8")
            if prev == pres:
                pass
            else:
                #print("Type:", obj.type)
                #print("Data: ", obj.data)
                #print("Type:", type(item_value))
                #print(item_value)
                prev = pres
#Bill calculation
                #press = item_value
                print(item_value)
                item = item_value[:3]
                itemId = item_value[5:]

                
                if item_value in items:
                    if item_value not in bill:
                        bill.append(item_value)
                        for i in range(len(inventory)):
                            if inventory[i][0] == item:
                                add = int(inventory[i][1])
                                tot += add 
                                print(f"Total value: {tot}")
                                st_tot=str(tot)
                                send_message("Total: Rs."+st_tot)
                                break
                    else:
                        print("Item already in the bill")
                        send_message("Item already in the bill")
                else:
                    print("Item not found")
                    send_message("Item not found")

            cv2.putText(frame, str(obj.data), (50, 50), font, 2,
                        (255, 0, 0), 3)

        cv2.imshow("live transmission", frame)

    except urllib.error.URLError as e:
        print(f"Failed to connect to the server: {e.reason}")
        # Optionally break the loop or retry after some time
        break
    except Exception as e:
        print(f"An unexpected error occurred: {e}")
        break

    key = cv2.waitKey(1)
    if key == 27:
        break

cv2.destroyAllWindows()

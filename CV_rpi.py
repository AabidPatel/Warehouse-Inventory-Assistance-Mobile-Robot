import cv2
import numpy as np
import cv2.aruco as aruco
import RPi.GPIO as GPIO
import time
import serial

odd_tag = []
even_tag = []

def aruco_detect():
    # Define the video capture object
    cap = cv2.VideoCapture(0)

    # Create a dictionary of ArUco markers
    #aruco_dict = aruco.Dictionary_create(aruco.DICT_6X6_250)
    aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_6X6_250)

    # Create a detector parameters object
    parameters = aruco.DetectorParameters()
    
    ids = []

    while True:
        # Read a frame from the video capture
        print("Getting aruco")
        y = GPIO.input(23)
        if y==0:
            #ser_print(chr(18))
            print("BROKEN Y")
            break
        elif y==1:
            pass
        ret, frame = cap.read()

        # Convert the frame to grayscale
        gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        detector = aruco.ArucoDetector(aruco_dict, parameters)
        # Detect the ArUco markers in the image
        #corners, ids, _ = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
        
        corners, ids, _ = detector.detectMarkers(gray)
        if len(corners) > 0:
        # flatten the ArUco IDs list
            ids = ids.flatten()
            for i in range(len(ids)):
                #print(ids[i])
                if ids[i] % 2 == 0:
                    if even_tag.count(ids[i])==0:
                        even_tag.append(ids[i])
                        print(even_tag)
                        time.sleep(1)
                elif ids[i] % 2 != 0:
                    if odd_tag.count(ids[i])==0:
                        print("HELLO")
                        odd_tag.append(ids[i])
                        print(odd_tag)
                        #y= GPIO.input(23)
                        time.sleep(1)
                        
            #print(ids)
            #ids = []
            #if len(ids)==2:
                
            
            # output pin high to finish scan
            # Once you get the data in finish scan then make the y ouput pin from propeller low
            #break

        # Draw the detected markers on the frame
        frame_markers = aruco.drawDetectedMarkers(frame.copy(), corners, ids)

        # Display the frame with the detected markers
        cv2.imshow('ArUco Markers', frame_markers)

        # Exit the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the video capture object and close all windows
    cap.release()
    cv2.destroyAllWindows()




def triangle_direction():
    cap = cv2.VideoCapture(0)
    # Read a frame from the video capture
    print("Getting frame")
    
    r = 0
    l = 0 
    '''def nothing(x):
        pass

    cv2.namedWindow("result_mask")

    # Create trackbars for HSV values
    cv2.createTrackbar("Hue min", "result_mask", 98, 255, nothing)
    cv2.createTrackbar("Hue max", "result_mask", 140, 255, nothing)
    cv2.createTrackbar("Sat min", "result_mask", 166, 255, nothing)
    cv2.createTrackbar("Sat max", "result_mask", 236, 255, nothing)
    cv2.createTrackbar("Val min", "result_mask", 85, 255, nothing)
    cv2.createTrackbar("Val max", "result_mask", 255, 255, nothing)'''

    # Load image
    while True:
        ret, img = cap.read()
        # Convert to grayscale
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Denoise the image
        blur = cv2.GaussianBlur(gray, (5, 5), 0)

        # Create a binary mask of blue pixels
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)
        
        '''h_min = cv2.getTrackbarPos("Hue min", "result_mask")
        h_max = cv2.getTrackbarPos("Hue max", "result_mask")
        s_min = cv2.getTrackbarPos("Sat min", "result_mask")
        s_max = cv2.getTrackbarPos("Sat max", "result_mask")
        v_min = cv2.getTrackbarPos("Val min", "result_mask")
        v_max = cv2.getTrackbarPos("Val max", "result_mask")'''
        
        lower_blue = np.array([98, 122, 85])
        upper_blue = np.array([115, 250, 255])
        
        mask = cv2.inRange(hsv, lower_blue, upper_blue)

        # Apply the mask to the denoised image
        masked = cv2.bitwise_and(blur, blur, mask=mask)

        # Detect edges in the masked image
        edges = cv2.Canny(masked, 50, 200)

        # Find contours in the edges image
        contours, hierarchy = cv2.findContours(edges, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)

        # Filter contours based on shape and size
        triangles = []
        for cnt in contours:
            approx = cv2.approxPolyDP(cnt, 0.01 * cv2.arcLength(cnt, True), True)
            if len(approx) == 3 and cv2.contourArea(cnt) > 200:
                triangles.append(approx)

        # Draw the detected triangles on the original image
        for triangle in triangles:
            x, y, w, h = cv2.boundingRect(triangle)
            cv2.rectangle(img, (x, y), (x + w, y + h), (0, 255, 0), 2)

            # Determine the direction of the triangle
            if triangle[0][0][0] < triangle[1][0][0] < triangle[2][0][0]:
                print('Right facing triangle')
                # Set pin high for right facing triangle
                GPIO.output(18, GPIO.HIGH)
                r = 1
                #time.sleep(1)
                #x = 0
                break
                #return
            elif triangle[0][0][0] > triangle[1][0][0] > triangle[2][0][0]:
                print('Left facing triangle')
                
                GPIO.output(17, GPIO.HIGH)
                # Set pin high for left facing triangle
                l = 1
                #x = 0
                break
                #return
           
        if r==1 or l ==1:
            #GPIO.output(18, GPIO.LOW)
            #GPIO.output(17, GPIO.LOW)
            print(r,l)
            time.sleep(1)
            
            #GPIO.cleanup()
            cap.release()
            cv2.destroyAllWindows()
            break
        elif r ==0 or l==0:
            pass
        # Display the result
        #cv2.imshow('result_mask', masked)
        cv2.imshow('result', img)
        
        #Exit the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
    
    #GPIO.cleanup()
    cap.release()
    cv2.destroyAllWindows()        
    #return

def NoMansLand():
    triangle_direction()
    
    print("Triangle Detected")
    
    cap = cv2.VideoCapture(0)

    # Create a dictionary of ArUco markers
    #aruco_dict = aruco.Dictionary_create(aruco.DICT_6X6_250)
    aruco_dict = aruco.getPredefinedDictionary(aruco.DICT_6X6_250)

    # Create a detector parameters object
    parameters = aruco.DetectorParameters()
    ids = []
    
    # Define the coordinates of the top-left and bottom-right corners of the cropping rectangle
    x, y, w, h = 100, 100, 200, 200
    
    # Loop through the video frames
    while True:
        # Read a frame from the video capture object
        ret, frame = cap.read()
        
        # Crop the frame to the specified rectangle
        cropped_frame = frame[y:y+h, x:x+w]
        
        # Display the cropped frame
        #cv2.imshow("Cropped Frame", cropped_frame)
        
        # Convert the frame to grayscale
        gray = cv2.cvtColor(cropped_frame, cv2.COLOR_BGR2GRAY)
        detector = aruco.ArucoDetector(aruco_dict, parameters)
        # Detect the ArUco markers in the image
        #corners, ids, _ = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
        
        corners, ids, _ = detector.detectMarkers(gray)
        
        if len(corners) > 0:
        # flatten the ArUco IDs list
            ids = ids.flatten()
            if len(ids) != 0:
                print("10 is HIGH")
                GPIO.output(10,GPIO.HIGH)
                time.sleep(1)
                print("DOCK DETECTED")
                break
            elif len(ids) ==0:
                pass
            
        else:
            pass
        
        # Draw the detected markers on the frame
        frame_markers = aruco.drawDetectedMarkers(frame.copy(), corners, ids)

        # Display the frame with the detected markers
        cv2.imshow('ArUco Markers', frame_markers)
        cv2.imshow('Cropped frame' , cropped_frame)

        # Exit the loop if 'q' is pressed
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Release the video capture object and close all windows
    cap.release()
    cv2.destroyAllWindows()
    
def serial_print(msg1,msg2):

    ser = serial.Serial(port='/dev/serial0' , baudrate= 9600 )
    ser.close()
    ser.open()
    while ser.isOpen():
        ser.write(chr(12).encode())
        #ser.write(chr(17).encode())
        ser.write(msg1.encode()+str(msg2).encode())
        time.sleep(2)
        ser.write(chr(12).encode())
        ser.close()
    

while(1):
    GPIO.setmode(GPIO.BCM)
    GPIO.setup(18, GPIO.OUT)
    GPIO.setup(17, GPIO.OUT)
    GPIO.setup(10, GPIO.OUT)
    
    GPIO.setup(22,GPIO.IN)
    GPIO.setup(23,GPIO.IN)
    GPIO.setup(27,GPIO.IN)
    
    # Add input pins in while loop to update
    #x = GPIO.setup(22,GPIO.IN)
    #y = GPIO.setup(23,GPIO.IN)
    #print(r,l,x,y)
    #time.sleep(1)
    x = GPIO.input(22)
    y = GPIO.input(23)
    z = GPIO.input(27)
    #define z
    
    print("x = ",x)
    print("y = " ,y)
    print("z = ",z)
    #y = GPIO.input(23)
    if x==1:
        print("HI")
        #cap = cv2.VideoCapture(0)
        triangle_direction()
        #GPIO.output(18, GPIO.LOW)
        print("Triangle detected")
        while (x == 1):
            print("change karo yaar")
            x = GPIO.input(22)
    
    elif y==1:
        print("WHYYYY")
        prev_odd_count = len(odd_tag)
        prev_even_count = len(even_tag)
        
        aruco_detect()
        
        new_odd_count = len(odd_tag)
        new_even_count = len(even_tag)
        
        d = new_odd_count - prev_odd_count
        nd= new_even_count - prev_even_count
        
        print("d = ", d)
        serial_print("Defective= " , d)
        print("nd = " , nd)
        serial_print("NonDefective= ",nd)
        
        time.sleep(2)
        
    elif z==1:
        NoMansLand()
        GPIO.output(10, GPIO.LOW)
        time.sleep(7)
        serial_print("DEFECTIVE " , len(odd_tag))
        serial_print("NON-DEFECTIVE " , len(even_tag))
        while(z==1):
            print("DOCKING DONE")
        
    elif x==0:
        print("Pins are zero")
        GPIO.output(18, GPIO.LOW)
        GPIO.output(17, GPIO.LOW)
        time.sleep(1)      
        
    elif y==0:
        print("ARUCO DETECTED")
        
    elif z==0:
        print("PROJECT OVER")

# Release the video capture object and close all windows
#print("Break!!")
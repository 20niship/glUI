import cv2
import numpy as np

def main():
    img = cv2.imread("icons.png")
    # gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY) 

    Zoom_ratio = 0.5
    
    _, _, gray = cv2.split(img)
    height = gray.shape[0]
    width  = gray.shape[1]
    # gray = cv2.resize(gray , (int(width*Zoom_ratio), int(height*Zoom_ratio)))
    gray = cv2.resize(gray , (23*48, 8*48))

    height = gray.shape[0]
    width  = gray.shape[1]
    mylist = [0]*int(np.size(gray)/8)

    for i in range(height):
        for j in range(width):
            index = i*width+j
            if gray[i][j] > 75:
                mylist[int(index/8)] |= 2**(7-index%8)
                print("#", end="")
            else:
                print(" ", end ="")
        print("")

            
    print("sizeof icon texure = ", len(mylist))
    print("width = ", width )
    print("width = ", height)
    L = 0
    for i in mylist:
        print(hex(i)+",", end="")
        L+=1
        if L%width == 0:
            print("")

main()


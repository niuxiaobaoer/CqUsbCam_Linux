
LIBS = -L/usr/local/lib \
    -lusb-1.0
LIBS += -L/home/nxb/Desktop/USBCamera/USB30/FX3_SDK_Linux/FX3_SDK_Linux_v1.3.3/cyusb_linux_1.0.4/lib \
    -lcyusb

all: _X_64_ 


_X_64_: CqU3Cam.cpp
	g++ -g -O0 -Wall CqU3Cam.cpp -fPIC -shared $(LIBS) -o ./x64/libCqU3Cam.so

#_X_86_: CqU3Cam.cpp
#	g++ -g -m32 -O0 -Wall CqU3Cam.cpp -fPIC -shared $(LIBS) -o ./x86/libCqU3Cam.so

clean: 
	rm ./x64/*.so

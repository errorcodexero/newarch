#pragma once

#include <stdio.h>
#include <sys/ioctl.h>
#include <stdlib.h>
#include <string>
#include <cstring>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <chrono>

namespace frc {
    class SerialPort {
        public:
            enum Port { 
                kOnboard = 0, 
                kMXP = 1, 
                kUSB = 2, 
                kUSB1 = 2, 
                kUSB2 = 3 
             } ;

        public:
            SerialPort(int baudRate, Port id) {
                Init(baudRate, id) ;
            }

            virtual ~SerialPort() {            
            }

            void SetReadBufferSize(int size) {
                this->ReadBufferSize = size;
            }

            void SetTimeout(int timeout) {
                this->timeout = timeout;
                tty.c_cc[VTIME] = timeout*10;
                cfmakeraw(&tty);
                if(tcsetattr(this->fd,TCSANOW,&tty) != 0) std::cout << "Failed to initialize serial in SetTimeout." << std::endl;;
            }


            void EnableTermination(char c) {
                this->termination = true;
                this->terminationChar = c;
            }

            void Flush() {
                tcflush(this->fd, TCOFLUSH);
            }        

            int Write(char *data, int length) {
                int n_written = 0, spot = 0;
                do {
                    n_written = write( this->fd, &data[spot], length );
                    if (n_written > 0)
                        spot += n_written;
                } while (data[spot-1] != terminationChar); 

                return n_written ;
            }        

            int GetBytesReceived() {
                int bytes_avail;
                ioctl(this->fd, FIONREAD, &bytes_avail);
                return bytes_avail;
            }        

            int Read(char *data, int size)  ;
                
            void Reset() {
                tcflush(this->fd, TCIOFLUSH);
            }

            void Close() {
                close(this->fd);
            }

        private:
            void Init(int baudRate, Port id) ;
            void WaitForData() ;

        private:
            int ReadBufferSize;
            int timeout;
            char terminationChar;
            bool termination;
            Port id;
            int baudRate;
            int fd;
            struct termios tty;
            int err;
    };
}


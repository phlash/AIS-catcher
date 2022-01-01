/*
Copyright(c) 2021 jvde.github@gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once

#include "Device.h"

#ifdef HASRTLTCP

#include <fstream>
#include <iostream>

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netdb.h>
#define SOCKET int
#endif

#endif

namespace Device {

	// to be expanded with device specific parameters and allowable parameters (e.g. sample rate, gain modes, etc)
	class SettingsRTLTCP : public DeviceSettings
	{
	private:

		int freq_offset = 0;
		bool tuner_AGC = true;
		bool RTL_AGC = false;
		FLOAT32 tuner_Gain = 33.0;
		bool bias_tee = false;
		std::string host = "127.0.0.1";
		std::string port = "1234";

	public:

		friend class RTLTCP;

		void Print();
		void Set(std::string option, std::string arg);
	};

	class RTLTCP : public DeviceBase
	{
#ifdef HASRTLTCP
		SOCKET sock = -1;

		std::string host;
		std::string port;
		struct addrinfo* address = NULL;

		struct {
			uint32_t magic = 0;
			uint32_t tuner = 0;
			uint32_t gain = 0;
		} dongle;

		// output vector

		static const int TRANSFER_SIZE = 1024;
		static const int BUFFER_SIZE = 16 * 16384;

		std::thread async_thread;
		std::thread run_thread;

		void RunAsync();
		void Run();

		FIFO<char> fifo;

		void setTuner_GainMode(int);
		void setTuner_Gain(FLOAT32);
		void setRTL_AGC(int);
		void setFrequencyCorrection(int);

		void setParameter(uint8_t cmd, uint32_t param);

	public:

		RTLTCP();
		~RTLTCP();

		// Control
		void Close();
		void Play();
		void Stop();

		bool isCallback() { return true; }

		static void pushDeviceList(std::vector<Description>& DeviceList);

		// Device specific
		void Open(uint64_t h, SettingsRTLTCP& s);
		void applySettings(SettingsRTLTCP& s);
#endif
	};
}
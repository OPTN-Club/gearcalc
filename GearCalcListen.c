#define WIN32_LEAN_AND_MEAN

#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <blank_html.h>
#include "GearCalcListen.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define BUFLEN 328	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data
#define NUM_DATA_POINTS 30 //number of data points to collect
#define RPM_TOLERANCE 100

int main()
{
	float rpm_values[NUM_DATA_POINTS] = { 0 };
	float torque_values[NUM_DATA_POINTS] = { 0 };
	int num_collected = 0;
	int curr_car_ordinal = -1;


	SOCKET s;
	WORD version_requested;
	struct sockaddr_in server, si_other;
	int slen, recv_len;
	char buf[BUFLEN];
	WSADATA wsa_data;
	version_requested = MAKEWORD(2, 2);


	slen = sizeof(si_other);

	int wsa_error = WSAStartup(version_requested, &wsa_data);
	if (wsa_error != 0) {
		printf("Could not initialize socket dll: %d\n", wsa_error);
		return -1;
	}
	s = socket(AF_INET, SOCK_DGRAM, 0);
	if (s == INVALID_SOCKET) {
		printf("Failed to create socket: %d", WSAGetLastError());
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(PORT);




	if (bind(s, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		return -1;
	}

	puts("Bind done");
	while (1)
	{

		fflush(stdout);
		//clear the buffer by filling null, it might have previously received data
		memset(buf, "", BUFLEN);

		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			return -1;
		}

		//print details of the client/peer and the data received
//		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		struct forza_data_packet* latest_packet = (struct forza_data_packet *)buf;
//		printf("rpm %f\n torque %f\n", latest_packet->current_rpm, latest_packet->engine_torque);
		//this needs to be done once per car
		if (latest_packet->car_ordinal != curr_car_ordinal) {
			printf("New car!\n");
			curr_car_ordinal = latest_packet->car_ordinal;
			float rpm_range = (latest_packet->max_rpm*.9) - latest_packet->idle_rpm;
			float step_size = rpm_range / NUM_DATA_POINTS;
			for (int i = 0; i < NUM_DATA_POINTS; i++) {
				rpm_values[i] = latest_packet->idle_rpm + (step_size * i);
			}
		}
		for (int i = 0; i < NUM_DATA_POINTS; i++) {
			if (torque_values[i] == 0 && latest_packet->engine_torque > 0 && latest_packet->accelerator_position == -1) {
				if (latest_packet->current_rpm<rpm_values[i] + RPM_TOLERANCE && latest_packet->current_rpm>rpm_values[i] - RPM_TOLERANCE) {
					//store this value
					torque_values[i] = latest_packet->engine_torque;
					rpm_values[i] = latest_packet->current_rpm;
					num_collected++;
				}
			}
		}
		for (int i = 0; i < NUM_DATA_POINTS; i++) {
			if (torque_values[i] == 0 && rpm_values[i] > 0) {
				printf("missing rpm: %f\n", rpm_values[i]);
			}
		}
		if (num_collected == NUM_DATA_POINTS) {
			break;
		}


	}
	//all data collected, print it here
	printf("Done collecting! Here's your data.\n");

	int i = 0;
	size_t last_offset = NULL;
	char* search_offset = NULL;
	while((search_offset = strstr(blank_html_data+last_offset+15, "\"torque_curve\":")) != NULL){
		printf("%p\n", search_offset);
		last_offset = blank_html_data - search_offset;
	}

	OFSTRUCT outstruct;
	HFILE out_file = OpenFile("notblank.html", &outstruct, OF_CREATE);
	printf("%s\n", blank_html_data);
	char* pos = strstr(blank_html_data, "{\"x\":[1]");
	if (pos == NULL) {
		printf("HTML is wrong\n");
		return -1;
	}
	size_t offset = pos - (char*)blank_html_data;
	size_t end_of_data = offset + 17;
	int float_len = 0;
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		int tmp_len = snprintf(NULL, 0, "%f", torque_values[i]) + 1;
		if (tmp_len > float_len) {
			float_len = tmp_len;
		}
	}
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		int tmp_len = snprintf(NULL, 0, "%f", torque_values[i]) + 1;
		if (tmp_len > float_len) {
			float_len = tmp_len;
		}
	}
	float_len = float_len + 5;
	printf("%d\n", float_len);
	char* out_buffer = malloc(float_len*sizeof(char)*NUM_DATA_POINTS*2 + blank_html_size);
	if (out_buffer == NULL) {
		printf("out of memory\n");
		return -1;
	}
	memset(out_buffer, 0, float_len * sizeof(char) * NUM_DATA_POINTS * 2 + blank_html_size);
	memcpy(out_buffer, blank_html_data, offset);

	lstrcat(out_buffer, "{\"x\":[");

	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		int len = snprintf(NULL, 0, "%f", rpm_values[i]);
		char* result = (char*)malloc(len+1);
		if (result == NULL) {
			printf("out of memory\n");
			return -1;
		}
		snprintf(result, len+1, "%f", rpm_values[i]);
		lstrcat(out_buffer, result);
		if (i + 1 != NUM_DATA_POINTS) {
			lstrcat(out_buffer, ",");
		}
		free(result);
	}

    lstrcat(out_buffer, "],\"y\":[");
	for (int i = 0; i < NUM_DATA_POINTS; i++) {
		int len = snprintf(NULL, 0, "%f", torque_values[i]);
		char* result = (char*)malloc(len + 1);
		if (result == NULL) {
			printf("out of memory\n");
			return -1;
		}
		snprintf(result, len+1, "%f", torque_values[i]);
		lstrcat(out_buffer, result);
		if (i + 1 != NUM_DATA_POINTS) {
			lstrcat(out_buffer, ",");
		}
		free(result);
	}
	lstrcat(out_buffer, "]}");

	lstrcat(out_buffer, blank_html_data + end_of_data);
	WriteFile(out_file, out_buffer, strlen(out_buffer), NULL, NULL);
	free(out_buffer);
	CloseHandle(out_file);
	system((char*)outstruct.szPathName);

	closesocket(s);
	WSACleanup();

	return 0;
}

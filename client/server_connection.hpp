// Copyright (c) 2013-2016 Trillek contributors. See AUTHORS.txt for details
// Licensed under the terms of the LGPLv3. See licenses/lgpl-3.0.txt

#pragma once

#include <asio.hpp>
#include <spdlog/spdlog.h>
#include <memory>
#include <thread>
#include <chrono>
#include <list>
#include <iostream>
#include <atomic>
#include "server-message.hpp"

using asio::ip::tcp;

namespace tec {
	extern std::map<tid, std::function<void(const proto::Entity&, const proto::Component&)>> in_functors;
	extern std::map<tid, std::function<void(const proto::Entity&, const proto::Component&, const state_id_t)>> update_functors;

	class Simulation;

	namespace networking {
		extern const char* SERVER_PORT_STR;
		extern const char* LOCAL_HOST;
		typedef std::chrono::milliseconds::rep ping_time_t;

		// Used to connect to a server.
		class ServerConnection {
		public:
			ServerConnection(Simulation& simulation);

			bool Connect(std::string ip = LOCAL_HOST); // Connects to a server.

			void Disconnect(); // Closes the socket connection and stops the read and sync loops.

			void Stop(); // Stop read and sync loops.

			void StartRead(); // Starts the read loop.

			void StartSync(); // Starts the sync loop.

			void SendChatMessage(std::string message); // Send a ServerMessage with type CHAT_MESSAGE.

			void Send(ServerMessage& msg);

			// Gets the last received state ID.
			state_id_t GetLastRecvStateID() {
				return this->last_received_state_id;
			}

			// Get a list of recent pings.
			std::list<ping_time_t> GetRecentPings() {
				std::lock_guard<std::mutex> recent_ping_lock(recent_ping_mutex);
				return this->recent_pings;
			}

			// Returns the average ping.
			ping_time_t GetAveragePing() {
				return this->average_ping;
			}

			// Get the client ID assigned by the server.
			eid GetClientID() {
				return this->client_id;
			}

			// Register a message handler for a given MessageType.
			void RegisterMessageHandler(MessageType type, std::function<void(const ServerMessage&)> handler) {
				this->message_handlers[type] = std::move(handler);
			}
		private:
			void read_body(); // Used by the read loop. Calls read_header after the whole body is read.
			void read_header(); // Used by the read lop. Calls read_body after the header section is read.

			void SyncHandler(const ServerMessage& message);
			void GameStateUpdateHandler(const ServerMessage& message);

			static std::shared_ptr<spdlog::logger> _log;

			// ASIO variables
			asio::io_service io_service;
			asio::ip::tcp::socket socket;

			// Read loop variables
			ServerMessage current_read_msg;
			std::atomic<bool> stopped;

			// Ping variables
			std::chrono::high_resolution_clock::time_point sync_start, recv_time;
			std::list<ping_time_t> recent_pings;
			static std::mutex recent_ping_mutex;
			ping_time_t average_ping = 0;

			// Server-assigned client ID
			eid client_id = 0;

			// State management variables
			state_id_t last_received_state_id;
			Simulation& simulation;

			std::unordered_map<MessageType, std::function<void(const ServerMessage&)>, std::hash<std::underlying_type<MessageType>::type>> message_handlers;
		};
	}
}

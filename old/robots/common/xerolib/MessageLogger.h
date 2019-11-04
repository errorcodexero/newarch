#pragma once

#include <string>
#include <list>
#include <memory>
#include <algorithm>

namespace xero
{
	namespace base
	{
		class MessageDestination;

		class MessageLogger
		{
		public:
			enum class MessageType
			{
				Error,
				Warning,
				Info,
				Debug,
			};

		public:
			MessageLogger();
			~MessageLogger();

			void enableModules(uint64_t mod)
			{
				m_modules_active |= mod;
			}

			void disableModules(uint64_t mod)
			{
				m_modules_active &= ~mod;
			}

			void addDestination(std::shared_ptr<MessageDestination> dest_p);
			void addDestination(const char *filename_p);
			void addDestination(const std::string &filename);
			void addStandardOutputDestination();

			void startMessage(MessageType type, uint64_t mod)
			{
				if (m_in_message)
				{
					std::runtime_error err("cannot start a new message while an existing message is still active");
					throw err;
				}

				m_type = type;
				m_message_is_filtered = false;
				m_in_message = true;

				if ((m_modules_active & mod) == 0)
					m_message_is_filtered = true;

				if (std::find(m_send_types.begin(), m_send_types.end(), type) == m_send_types.end())
					m_message_is_filtered = true;

				m_message.clear();
			}

			void endMessage();

			MessageLogger &operator<<(const char *value_p);
			MessageLogger &operator<<(const std::string &value);
			MessageLogger &operator<<(double value);
			MessageLogger &operator<<(int8_t value);
			MessageLogger &operator<<(int16_t value);
			MessageLogger &operator<<(int32_t value);
			MessageLogger &operator<<(int64_t value);
			MessageLogger &operator<<(uint8_t value);
			MessageLogger &operator<<(uint16_t value);
			MessageLogger &operator<<(uint32_t value);
			MessageLogger &operator<<(uint64_t value);

		protected:
			void SendMessage(MessageType type, const std::string &message);

		private:
			// If true, we are in the middle of a message
			bool m_in_message;

			// If true, the current message is filtered
			bool m_message_is_filtered;

			// The subsystem that are active
			uint64_t m_modules_active;

			// If in the middle of a message, the type of message
			MessageType m_type;

			// If in the middle of a message, the text of the message so far
			std::string m_message;

			// The types of messages that actual get sent to the list of destinations
			std::list<MessageType> m_send_types;

			// The list of destinations
			std::list<std::shared_ptr<MessageDestination>> m_destinations;
		};
	}
}

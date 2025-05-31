import socket
import threading

SERVER_HOST = '127.0.0.1'
SERVER_PORT = 8080
CODING_TYPE = "ascii"

is_connected = True


def listen_for_messages(connection):
    global is_connected
    while is_connected:
        try:
            received_data = connection.recv(1024)

            if not received_data:
                print("[INFO] Server has closed the connection.")
                break

            print(received_data.decode(CODING_TYPE))
        except Exception as error:
            if not is_connected:
                return
            print(f"[ERROR] Error receiving message: {error}")
            break


def transmit_messages(connection, username):
    global is_connected
    while is_connected:
        try:
            outgoing_message = input()
            if outgoing_message.strip().lower() == '/q':
                print("[INFO] Disconnecting from server.")
                is_connected = False
                connection.close()
                break

            formatted_message = f"{username}: {outgoing_message}"
            connection.send(formatted_message.encode(CODING_TYPE))
        except Exception as error:
            if not is_connected:
                return
            print(f"[ERROR] Error sending message: {error}")
            break


if __name__ == '__main__':
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_HOST, SERVER_PORT))

    username = input("Enter your username: ")
    print("Connected! Type messages (type '/q' to exit).")

    threading.Thread(target=listen_for_messages, args=(client_socket,), daemon=True).start()
    transmit_messages(client_socket, username)

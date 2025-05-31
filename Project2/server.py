import socket
import threading

SERVER_HOST = ""
SERVER_PORT = 8080
CODING_TYPE = "ascii"

output_lock = threading.Lock()
client_list_lock = threading.Lock()
connected_clients = []


def handle_client(client_socket, client_address):
    with output_lock:
        print(f"[{threading.current_thread().name}] Connected to: {client_address[0]}:{client_address[1]}")
    try:
        welcome_message = "Welcome to the server!"
        client_socket.send(welcome_message.encode(CODING_TYPE))

        while True:
            message = client_socket.recv(1024)
            if not message:
                break

            with client_list_lock:
                for other_client in connected_clients:
                    try:
                        other_client.send(message)
                    except Exception as e:
                        with output_lock:
                            print(f"[{threading.current_thread().name}] Error: {e}")

    except Exception as e:
        with output_lock:
            print(f"[{threading.current_thread().name}] Error: {e}")

    finally:
        with client_list_lock:
            if client_socket in connected_clients:
                connected_clients.remove(client_socket)

        client_socket.close()
        with output_lock:
            print(f"[{threading.current_thread().name}] Disconnected from: {client_address[0]}:{client_address[1]}")


if __name__ == '__main__':
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((SERVER_HOST, SERVER_PORT))
    print("Socket bound to port", SERVER_PORT)
    server_socket.listen(5)
    print(f"Server listening on port {SERVER_PORT}...")

    while True:
        client_conn, client_addr = server_socket.accept()
        with client_list_lock:
            connected_clients.append(client_conn)
        threading.Thread(target=handle_client, args=(client_conn, client_addr), daemon=True).start()

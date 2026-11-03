import socket, time
# zwolnij kazdy wiszacy "nc -l -p PORT" w sesjach telefonu: polacz sie i zamknij
for port in list(range(2324, 2335)):
    try:
        s = socket.create_connection(("172.16.42.1", port), timeout=1.0); s.close(); print("zwolniono", port)
    except Exception: pass
    time.sleep(0.3)

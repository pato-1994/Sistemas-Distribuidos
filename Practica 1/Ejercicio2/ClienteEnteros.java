import java.io.*;
import java.net.*;
import java.util.Scanner;

public class ClienteEnteros {
    public static void main(String[] args) {
        if (args.length != 2) {
            System.err.println("Uso: java ClienteEnteros <IP_Servidor> <Puerto>");
            System.exit(1);
        }

        String ipServidor = args[0];
        int puerto = Integer.parseInt(args[1]);

        try (
            Socket socket = new Socket(ipServidor, puerto);
            PrintWriter escritor = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader lector = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            Scanner teclado = new Scanner(System.in)
        ) {
            System.out.println("Conectado al servidor en C (" + ipServidor + ":" + puerto + ")");
            System.out.println("Ingresa numeros enteros (Ingresa 0 para salir):");

            while (true) {
                System.out.print("\nEscribe un numero: ");
                if (!teclado.hasNextInt()) {
                    System.out.println("Por favor ingresa un numero entero valido.");
                    teclado.next();
                    continue;
                }

                int num = teclado.nextInt();

                // Enviar el número al servidor con delimitador de salto de línea
                escritor.println(num);

                // Si se envía 0, finaliza el programa
                if (num == 0) {
                    System.out.println("Se envio 0. Terminando programa...");
                    break;
                }

                // Leer e imprimir la respuesta incrementada desde el servidor
                String respuesta = lector.readLine();
                if (respuesta != null) {
                    System.out.println("El servidor C respondio: " + respuesta);
                } else {
                    System.out.println("El servidor cerro la conexion.");
                    break;
                }
            }
        } catch (UnknownHostException e) {
            System.err.println("Host desconocido: " + ipServidor);
        } catch (IOException e) {
            System.err.println("Error de comunicacion: " + e.getMessage());
        }
    }
}
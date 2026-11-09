import java.net.*; 
import java.io.*; 

public class Servidor_Eco {
    
    public static void main(String[] args) throws IOException {
        
        if (args.length != 1) {
            System.err.println("Uso desde consola:  <numero puerto>");
            System.exit(1);
        }
        
        int numeroPuerto = Integer.parseInt(args[0]);
        
        try (
            ServerSocket socketdelServidor = new ServerSocket(numeroPuerto);
            Socket socketdelCliente = socketdelServidor.accept();     
            PrintWriter escritor = new PrintWriter(socketdelCliente.getOutputStream(), true);                   
            BufferedReader lector = new BufferedReader(new InputStreamReader(socketdelCliente.getInputStream()));
        ) {
            String linealeida;
            // El ciclo lee lo que manda el cliente hasta que se desconecta
            while ((linealeida = lector.readLine()) != null) {
                System.out.println("El cliente envio: " + linealeida);
                
                // Aquí cambiamos el "eco" por la respuesta requerida en la tarea
                escritor.println("Hola que tal");
            }
        } catch (IOException e) {
            System.out.println(" ocurrio una excepcion cuando intentamos escuchar "
                + numeroPuerto + " o esperando por una conexicon");
            System.out.println(e.getMessage());
        }
    }
}
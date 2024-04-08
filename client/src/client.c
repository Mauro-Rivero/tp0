#include "client.h"

int main(void)
{
	/* Parte de logger
	t_log* log_tp0 = log_create("tp0.log", "PruebaLog", true, LOG_LEVEL_INFO);
	log_info(log_tp0, "Soy un Log");
	log_destroy(log_tp0);
	*/

	/*parte de config

	t_config* log_config = config_create("cliente.config");
	char* valor_key_string = config_get_string_value(log_config, "CLAVE");

	t_log* log_tp0 = log_create("tp0.log", "PruebaLog", true, LOG_LEVEL_INFO);
	log_info(log_tp0, valor_key_string);
	log_destroy(log_tp0);
	*/

// parte de leer la consola 
/*
	t_log* log_tp0 = log_create("tp0.log", "Leido en consola", true, LOG_LEVEL_INFO);
	char* leido_consola = readline("> ");
	log_info(log_tp0, leido_consola);
	log_destroy(log_tp0);
	free(leido_consola);
*/
//parte de strings
/*
	printf("EScribi todas las lineas que quieras para que se logueen");
	t_log* log_tp0 = log_create("tp0.log", "Leido en consola", true, LOG_LEVEL_INFO);
	char* leido_consola;
	do{
		leido_consola = readline("> ");
		if(strcmp(leido_consola, "") == 0){ //esto evita imprimir el log vacio, podria abstraerse la validación a una funcion "es vacio(unString)"
		}
		else{
			log_info(log_tp0, leido_consola);
		}
	} while (strcmp(leido_consola, "") != 0);

	printf("INGRESO UNA LINEA VACIA => EL PROGRAMA FINALIZARA.");
	log_destroy(log_tp0);
	free(leido_consola);
*/

	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	valor = config_get_string_value(config, "CLAVE");
	ip = config_get_string_value(config, "IP");
	puerto = config_get_string_value(config, "PUERTO");

	// Loggeamos el valor de config
	log_info(logger, "El valor de config es: %s", valor);


	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);
	log_info(logger, "Conexión creada");

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor, conexion);
	log_info(logger, "Handshaking enviado");

	// Armamos y enviamos el paquete;
	paquete(conexion);

	log_info(logger, "Proceso terminado correctamente. Se termina el programa.");
	terminar_programa(conexion, logger, config);

}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger;
	
	nuevo_logger = log_create("tp0.log", "Logging", true, LOG_LEVEL_INFO);
	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(nuevo_logger, "Hola! Soy un log");

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config;

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	nuevo_config = config_create("cliente.config");

	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;
	// La primera te la dejo de yapa
	leido = readline("> ");
	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while (strcmp(leido, "") != 0){
		log_info(logger, "El mensaje leído fue: '%s' " , leido);
		leido = readline("> ");
	}
	free(leido);
}

void paquete(int conexion)
{
	char* leido;
	t_paquete* paquete;
	paquete = crear_paquete();
	t_log* loggerPrueba = log_create("paquete.log", "Metiendo cosas al log", true, LOG_LEVEL_INFO);
	
	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while (strcmp(leido, "") != 0){
			log_info(loggerPrueba, "El mensaje leído fue: '%s'. Se va a agregar al paquete" , leido);
		agregar_a_paquete(paquete, leido, sizeof(leido) + 1);
			log_info(loggerPrueba, "Agregado al paquete satisfactoriamente.");
		leido = readline("> ");
	}
		log_info(loggerPrueba, "Se enviara el paquete.");
	enviar_paquete(paquete, conexion);
		log_info(loggerPrueba, "Paquete enviado satisfactoriamente.");

	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free(leido);
	log_destroy(loggerPrueba);
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	log_destroy(logger);
	liberar_conexion(conexion);
	config_destroy(config);
}

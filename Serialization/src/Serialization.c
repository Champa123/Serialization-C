
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct {
    int dni;
    int edad;
    int pasaporte;
    int long_nombre;
    char* nombre;
} t_persona;

typedef struct {
    //uint32_t size;  Tamaño del payload
	int size; // Tamaño del payload
    void* stream; // Payload
} t_buffer;

typedef struct {
    int codigo_operacion;
    t_buffer* buffer;
} t_paquete;

const int PERSONA = 0;



int calcularTamanioBufferPersona(t_persona* persona){

	 // DNI, Pasaporte y Edad y long_nombre son 4 ints + la long de la palabra +1 ("\0")
	return sizeof(int) * 4 + strlen(persona->nombre) + 1;
}

void* armarStream(t_persona* persona, int bufferSize){

	void* stream = malloc(bufferSize);
	int offset = 0; // Desplazamiento

	memcpy(stream + offset, &persona->dni, sizeof(int));					//Notar que se esta desplazando el puntero con el offset.
	offset += sizeof(int);
	memcpy(stream + offset, &persona->edad, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, &persona->pasaporte, sizeof(int));
	offset += sizeof(int);

	// Para el nombre primero mandamos el tamaño y luego el texto en sí:
	memcpy(stream + offset, &persona->long_nombre, sizeof(int));
	offset += sizeof(int);
	memcpy(stream + offset, persona->nombre, strlen(persona->nombre) + 1);
	// No tiene sentido seguir calculando el desplazamiento, ya ocupamos el buffer completo
	return stream;

}


t_buffer* crearBufferPersona(t_persona* persona){

	t_buffer* buffer = malloc(sizeof(t_buffer));

	buffer->size = calcularTamanioBufferPersona(persona);
	buffer->stream = armarStream(persona, buffer->size);

	return buffer;
}


t_paquete* crearPaquete(t_buffer* buffer){
	t_paquete* paquete = malloc(sizeof(t_paquete));

	paquete->codigo_operacion = PERSONA; // Podemos usar una constante por operación
	paquete->buffer = buffer; // Nuestro buffer de antes.

	return paquete;
}

void* prepararParaEnviarPaquete(t_buffer* buffer, t_paquete* paquete){
	void* a_enviar = malloc(buffer->size + sizeof(int) + sizeof(int)); //aca un int es del codigo de operacion y el otro es del tipo de size del buffer
		int offset = 0;

		memcpy(a_enviar + offset, &(paquete->codigo_operacion), sizeof(int));
		offset += sizeof(int);
		memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(int));
		offset += sizeof(int);
		memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

		return a_enviar;
}

int servidor(int unSocket) {

	t_persona* persona = (t_persona*) malloc(sizeof(t_persona));

	t_buffer* buffer = crearBufferPersona(persona);

	//Aqui ya estaria serializado nuestra persona pero lo vamos a empaquetar, agregarle el header al principio asi ya viendo el header se sabe que es una persona.

	t_paquete* paquete = crearPaquete(buffer);

	void* a_enviar = prepararParaEnviarPaquete(buffer, paquete);

	// Por último enviamos
		send(unSocket, a_enviar, buffer->size + sizeof(int) + sizeof(int), 0);

		// No nos olvidamos de liberar la memoria que ya no usaremos
		free(persona);
		free(a_enviar);
		free(paquete->buffer->stream);
		free(paquete->buffer);
		free(paquete);






	return EXIT_SUCCESS;
}



t_persona* deserializar_persona(t_buffer* buffer) {
    t_persona* persona = malloc(sizeof(t_persona));

    void* stream = buffer->stream;
    // Deserializamos los campos que tenemos en el buffer
    memcpy(&(persona->dni), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(persona->edad), stream, sizeof(int));
    stream += sizeof(int);
    memcpy(&(persona->pasaporte), stream, sizeof(int));
    stream += sizeof(int);

    // Por último, para obtener el nombre, primero recibimos el tamaño y luego el texto en sí:
    memcpy(&(persona->long_nombre), stream, sizeof(int));
    stream += sizeof(int);
    persona->nombre = malloc(persona->long_nombre);
    memcpy(persona->nombre, stream, persona->long_nombre);

    return persona;
}

int cliente(int unSocket){
	t_paquete* paquete = malloc(sizeof(t_paquete));
	paquete->buffer = malloc(sizeof(t_buffer));

	// Primero recibimos el codigo de operacion
	recv(unSocket, &(paquete->codigo_operacion), sizeof(int), 0);

	// Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
	recv(unSocket, &(paquete->buffer->size), sizeof(int), 0);
	paquete->buffer->stream = malloc(paquete->buffer->size);
	recv(unSocket, paquete->buffer->stream, paquete->buffer->size, 0);

	// Ahora en función del código recibido procedemos a deserializar el resto
	switch(paquete->codigo_operacion) {
	    case 0:  //PERSONA:
	    	;
	        t_persona* persona = deserializar_persona(paquete->buffer);

	        // Hacemos lo que necesitemos con esta info
	        // Y eventualmente liberamos memoria
	        free(persona-> nombre);
	        free(persona);

	        break;
	    // Evaluamos los demás casos según corresponda
	}

	// Liberamos memoria
	free(paquete->buffer->stream);
	free(paquete->buffer);
	free(paquete);

	return EXIT_SUCCESS;
}

int main(){
	return EXIT_SUCCESS;
}

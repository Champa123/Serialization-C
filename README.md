# Serialization-C
Esta es una guía de una serialización en C.


  Serializacion surge a partir de la necesidad de ordenar como se dispone en memoria un TDA (Tipo de Dato Abstracto: Un struct o alguna clase que planteemos).
  Ya que hay algunas funciones como send() y recv()- Si vemos el prototipo de las funciones send() y recv(), requieren que les digamos cuantos bytes vamos a
  enviar o recibir y si usamos sizeof(miTDA) no tenemos garantia que el tamaño de mi struct coincida entre emisor y receptor
  (quien hace el send() y quien hace el recv() )
 
 
 
   El objetivo entonces es estandarizar este envio usando un protocolo
        
        { header, dni, pasaporte, nombre, edad }
        header = tipo de dato que vamos a enviar
    
 Lo que viene despues del header se conoce como payload en nuestro caso header va a ser Persona
 lo siguiente que recibiremos sera dni, pasaporte, nombre, edad.
 
 
 



task escape() {
    while(true) {
        // Comprobar si la tarea está activa (no está inhibida por otra).
        // Aviso: proteger acceso concurrente a variable/s.
        
        // SI no está inhibida:
            // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
            // hacer algo según el estado del entorno.
            
            // SI tiene que actuar:
                // Inhibir tareas niveles inferiores.
                // Actuar.
            // SINO
                // Desinhibir tareas niveles inferiores.
        
        // SINO
            // Inhibir tareas inferiores.
    }
}

task light() {
        // Comprobar si la tarea está activa (no está inhibida por otra).
        // Aviso: proteger acceso concurrente a variable/s.
        
        // SI no está inhibida:
            // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
            // hacer algo según el estado del entorno.
            
            // SI tiene que actuar:
                // Inhibir tareas niveles inferiores.
                // Actuar.
            // SINO
                // Desinhibir tareas niveles inferiores.
        
        // SINO
            // Inhibir tareas inferiores.
}

task follow_wall() {
        // Comprobar si la tarea está activa (no está inhibida por otra).
        // Aviso: proteger acceso concurrente a variable/s.
        
        // SI no está inhibida:
            // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
            // hacer algo según el estado del entorno.
            
            // SI tiene que actuar:
                // Inhibir tareas niveles inferiores.
                // Actuar.
            // SINO
                // Desinhibir tareas niveles inferiores.
        
        // SINO
            // Inhibir tareas inferiores.
}

task go_to_wall() {
        // Comprobar si la tarea está activa (no está inhibida por otra).
        // Aviso: proteger acceso concurrente a variable/s.
        
        // SI no está inhibida:
            // Leer SENSORIZACIÓN necesaria para determinar si la tarea tiene que
            // hacer algo según el estado del entorno.
            
            // SI tiene que actuar:
                // Inhibir tareas niveles inferiores.
                // Actuar.
            // SINO
                // Desinhibir tareas niveles inferiores.
        
        // SINO
            // Inhibir tareas inferiores.
}

task main() {
    // Start the tasks
    startTask(escape);
    startTask(light);
    startTask(follow_wall);
    startTask(go_to_wall);
    // Wait for the tasks to finish
    wait1Msec(100000);
}
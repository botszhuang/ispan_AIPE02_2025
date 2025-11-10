function pp ( x ) { 
    //alert("Hello World!");
    console.log (x) ; 
}

function isPrime(x) {
    //if (x   < 2) { return false; }
    //if (x === 2) { return true;  }
    if ( ( x % 2 ) == 0 ) { return false ; } 
           
    let iMax = Math.sqrt ( x ) +1 ;
    for ( let i = 3 ; i < iMax ; i+=2 ) {
        if ( (x % i) === 0 )  { return false ; }
    }
        return true ;
}
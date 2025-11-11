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

function getPrime100( prime ){
        const MAX = 100 ;
        prime.push ( 2 ) ;
        for ( let i = 3 ; i < MAX ; i++ ) {
          if ( isPrime( i ) === true ) { prime.push( i ) ; }
        }
}

function createCell ( cellArray , prime , i ){
    
    cellArray[ i ] = document.createElement ( 'div' ) ;
    cellArray[ i ].classList.add ( "cellDiv" ) ;
    //cellArray[ i ].textContent = prime [ i ] ;

    let textDiv = document.createElement ('div') ;
    textDiv.classList.add ("textDiv") ;
    textDiv.textContent = prime[i]; 
    cellArray [ i ].appendChild ( textDiv ) ;
}
function create_and_append_Cells ( cellArray , prime , start , end, mainDiv){
    
    for ( let i = start ; i < end && i < prime.length ; i++ ) {    
        createCell ( cellArray , prime , i ) ;
        mainDiv.appendChild ( cellArray [ i ] ) ;
    }
}

function cal_batchSize( mainDiv , num ){

        pp ( "main = " + mainDiv.clientHeight + ", " + mainDiv.clientWidth ) ;
        
        wSize = Math.floor ( mainDiv.clientWidth * num ) ;
        hSize = 2 ;
        batchSize = wSize * hSize ;
        pp ( "wSize = " + wSize + ", hSize = " + hSize ) ;

        return batchSize ;
}
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

function appendCells ( cellArray , prime , start , end, mainDiv){
    
    for ( let i = start ; i < end ; i++ ) {    
        cellArray[ i ] = document.createElement ( 'div' ) ;
        cellArray[ i ].classList.add ( "cellDiv" ) ;
        cellArray[ i ].textContent = prime [ i ] ;
        mainDiv.appendChild ( cellArray [ i ] ) ;
    }

}

function cal_batchSize( mainDiv , cellArray ){

        pp ( "main = " + mainDiv.clientHeight + ", " + mainDiv.clientWidth ) ;
        pp ( "cell = " + cellArray.clientHeight + ", " + cellArray.clientWidth ) ;
        
        wSize = Math.floor ( mainDiv.clientWidth / cellArray.clientWidth ) ;
        hSize = Math.floor( mainDiv.clientHeight / cellArray.clientHeight ) ;
        batchSize = wSize * hSize ;
        pp ( "wSize = " + wSize + ", hSize = " + hSize ) ;

        return batchSize ;
}
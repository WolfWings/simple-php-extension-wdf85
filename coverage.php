<?php

//wdf85_encode( false );
//wdf85_decode( false );

wdf85_encode( '!!!!!' );
wdf85_decode( ' ' );
wdf85_decode( '  ' );
wdf85_decode( '!!!!!!!' );
wdf85_decode( 'v!!!!' );
wdf85_decode( 't8W-w' );
wdf85_decode( 'v!!!' );
wdf85_decode( 't8W-' );

ob_start();
phpinfo( INFO_MODULES );
ob_clean();

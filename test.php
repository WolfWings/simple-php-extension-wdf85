<?php

$errorcode = 0;

// Combining the 4-byte with 3-byte and 1-byte tests
// Reduces the call count without impacting results
foreach ( array(
	'00000000000000 !!!!!!!!!'
,	'00000001000001 !!!!w!!!$'
,	'00000002000002 !!!!#!!!y'
,	'00000004000004 !!!!x!!!-'
,	'00000008000008 !!!!)!!!9'
,	'00000010000010 !!!!1!!!Q'
,	'00000020000020 !!!!A!!wz'
,	'00000040000040 !!!!b!!#7'
,	'00000080000080 !!!wL!!xN'
,	'00000100000100 !!!$w!!*y'
,	'00000200000200 !!!y#!!3-'
,	'00000400000400 !!!-x!!E9'
,	'00000800000800 !!!9)!!jQ'
,	'00001000001000 !!!Q1!w]z'
,	'00002000002000 !!wzA!$D7'
,	'00004000004000 !!#7b!yhM'
,	'00008000008000 !!xNL!{Yx'
,	'00010000010000 !!*yw!<<*'
,	'00020000020000 !!3-#!WW3'
,	'00040000040000 !!E9xw98E'
,	'00080000080000 !!jQ)#QOj'
,	'00100000100000 !w]z1&-)~'
,	'00200000200000 !$D7A+92B'
,	'00400000400000 !yhMb5QCd'
,	'00800000800000 !{YxLJzgQ'
,	'0100000001 !<<*w!<'
,	'0200000002 !WW3#!W'
,	'0400000004 w98Exw9'
,	'0800000008 #QOj)#Q'
,	'1000000010 &-)~1&-'
,	'2000000020 +92BA+9'
,	'4000000040 5QCdb5Q'
,	'8000000080 JzgQLJz'
,	'0000 !!!'
,	'0001 !!*'
,	'0002 !!3'
,	'0004 !!E'
,	'0008 !!j'
,	'0010 !w]'
,	'0020 !$D'
,	'0040 !yh'
,	'0080 !{Y'
,	'0100 !<<'
,	'0200 !WW'
,	'0400 w98'
,	'0800 #QO'
,	'1000 &-)'
,	'2000 +92'
,	'4000 5QC'
,	'8000 Jzg'
,	'00 !!'
,	'ffffffffffffff t8W-!t8W*'
,	'fffffffefffffe t8Wzvt8W&'
,	'fffffffdfffffd t8Wzut8W#'
,	'fffffffbfffffb t8Wzst8Vs'
,	'fffffff7fffff7 t8Wzot8Vg'
,	'ffffffefffffef t8Wzgt8VN'
,	'ffffffdfffffdf t8WzVt8Ut'
,	'ffffffbfffffbf t8Wz6t8Ti'
,	'ffffff7fffff7f t8W+Kt8RQ'
,	'fffffefffffeff t8W)vt8N#'
,	'fffffdfffffdff t8W&ut8Ds'
,	'fffffbfffffbff t8Vvst82g'
,	'fffff7fffff7ff t8Vjot7dN'
,	'ffffefffffefff t8VQgt6pt'
,	'ffffdfffffdfff t8V!Vt53i'
,	'ffffbfffffbfff t8Tl6t1fR'
,	'ffff7fffff7fff t8RTKt*ux'
,	'fffefffffeffff t8N&vss|v'
,	'fffdfffffdffff t8DvusVvm'
,	'fffbfffffbffff t82jsrv?Z'
,	'fff7fffff7ffff t7dQoq](6'
,	'ffefffffefffff t6q!gozNC'
,	'ffdfffffdfffff t53lVivE]'
,	'ffbfffffbfffff t1fU6^]4<'
,	'ff7fffff7fffff t*u(KJzgN'
,	'fefffffffe ss<#vsV'
,	'fdfffffffd sVvpus|'
,	'fbfffffffb rv?]srZ'
,	'f7fffffff7 q](9oqA'
,	'efffffffef ozNFgng'
,	'dfffffffdf ivEaViZ'
,	'bfffffffbf ^]4?6^A'
,	'7fffffff7f JzgQKIg'
,	'ffff t8N'
,	'fffe t8D'
,	'fffd t8|'
,	'fffb t8)'
,	'fff7 t7Z'
,	'ffef t6g'
,	'ffdf t5*'
,	'ffbf t1~'
,	'ff7f t*l'
,	'feff ss2'
,	'fdff sVm'
,	'fbff rv6'
,	'f7ff q~u'
,	'efff ozE'
,	'dfff iv<'
,	'bfff ^]+'
,	'7fff Jz]'
,	'ff ss'
) as $test ) {
	list( $hex, $enc ) = explode( ' ', $test );
	$dec = hex2bin( $hex );
	$enctest = wdf85_encode( $dec );
	if ( $enctest !== $enc ) {
		print 'Encoding test failure - for ' . bin2hex( $dec ) . ' expected ' . $enc . ' but got ' . $enctest . "\n";
		$errorcode = 1;
	}
	$dectest = wdf85_decode( $enc );
	if ( $dectest !== $dec ) {
		print 'Decoding test failure - for ' . $enc . ' expected ' . bin2hex( $dec ) . ' but got ' . bin2hex( $dectest ) . "\n";
		$errorcode = 1;
	}
}

if ( $errorcode === 0 ) {
	print 'All individual bit tests for all word lengths passed.' . "\n";
} else {
	exit( $errorcode );
}

if ( wdf85_decode( ' ' ) !== null ) {
	print 'Decoding test failure - invalid "mod 5 = 1" length input did not result in NULL response' . "\n";
	exit( 1 );
}

if ( wdf85_decode( 'v!!!!' ) !== null ) {
	print 'Decoding test failure - out-of-range (> 2^32 on final multiply) input did not result in NULL response' . "\n";
	exit( 1 );
}

if ( wdf85_decode( 't8W-w' ) !== null ) {
	print 'Decoding test failure - out-of-range (> 2^32 on final addition) input did not result in NULL response' . "\n";
	exit( 1 );
}

if ( wdf85_decode( 'v!!!' ) !== null ) {
	print 'Decoding test failure - out-of-range (> 2^32 on final multiply w/ truncated chunk) input did not result in NULL response' . "\n";
	exit( 1 );
}

if ( wdf85_decode( 't8W-' ) !== null ) {
	print 'Decoding test failure - out-of-range (> 2^32 on final addition w/ truncated chunk) input did not result in NULL response' . "\n";
	exit( 1 );
}

print 'All "NULL response" test cases passed.' . "\n";

exit( 0 );

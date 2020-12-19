<?php
/*
	Author: João calisto
	Description: Functions to create the login on WebService and sends the xml
*/
require_once('func.php');

// Exemplo Config $ws
$ws = array(
	'comp' => 5,
	'path' => 'eguarda/php/',
	//'conn' => 'http://www.averbeporto.com.br/websys/php/conn.php',
	'conn' => 'http://www.averbeporto.com.br/websys/?comp=5',
	'cookie' => tempnam(sys_get_temp_dir(), 'ws_'),
	'logged' => ''
);

/**
* Envia arquivo
*
* @param {String} Conteudo do arquivo
* @param {Array} Usuario e senha. Ex.: array('user'=>'USUARIO', 'pass'=>'SENHA', 'path'=>'')
* @param {String} (optional) Remetente (em caso de email)
* @return {Array} Retorna resposta do webservice
*/
function sendFile($sFileContent, $aUser, $sRecipient = ''){
	global $ws;
	$file = tmpfile();
	fwrite($file, $sFileContent);
	rewind($file);
	$meta = stream_get_meta_data($file);
	$mime = mime_content_type($meta['uri']);

	$post = array(
	'file' => (version_compare(PHP_VERSION, '5.5') >= 0)? new CURLFile($meta['uri'], $mime) : '@'.$meta['uri'].';type='.$mime
	);

	if ($sRecipient) {
		$post['recipient'] = $sRecipient;
	}

	// Login
	if ($ws['logged'] != $aUser['user']) {
		$res = json_decode(websysRequest($aUser), true);
		if (isset($res['logout']) && $res['logout']) {
				echo "falha no login\n" ;
				//ws_log('MAIL2EG: ['.$aUser['user'].']: '.posix_getpid().': Falha do login. ');
			}
	} else {
		$res['success'] = $res['C'] = true;
		echo "login feito\n";
	}

	// Upload
	if ($res['success'] && isset($res['C'])) {
		$ws['logged'] = $aUser['user'];
		$res = json_decode(websysRequest($post, 'Upload'), true);
		echo ("upload feito\n");
	}
	else
	{
		echo "erro no upload\n";
	}

	fclose($file);
	return $res;
}


$aUser = array(
		'comp' => 5,
        'user' => $argv[2],
        'pass' => $argv[3],
        'path' => ''
);
$sFileContent = file_get_contents($argv[1]);

$res = sendFile($sFileContent, $aUser);

print_r($res);
?>

<?php
/**
* Open an url on https using curl and return content
* @author hatem <info@phptunisie.net>
* Adapted by stdmedoth <jovictor210@gmail.com>
* use the LIBCURL based code to send the xml to webservice
*/
function open_https_url($url,$refer = "", $usecookie = false) {
    if ($usecookie) {

        if (file_exists($usecookie)) {

            if (!is_writable($usecookie)) {

                return "Can't write to $usecookie cookie file, change file permission to 777 or remove read only for windows.";
            }
        } else {
            $usecookie = ($usecookie === true)? "cookie.txt" : $usecookie;


            if (!touch($usecookie)) {

                return "Can't write to $usecookie cookie file, change file permission to 777 or remove read only for windows.";
            }
        }

    }


    $ch = curl_init();
    curl_setopt($ch, CURLOPT_URL, $url);
    curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, FALSE);
    curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 2);
    curl_setopt($ch, CURLOPT_HEADER, 0);
    curl_setopt($ch, CURLOPT_USERAGENT, "Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)");

    if ($usecookie) {
        curl_setopt($ch, CURLOPT_COOKIEJAR, $usecookie);

        curl_setopt($ch, CURLOPT_COOKIEFILE, $usecookie);
    }

    if ($refer != "") {

        curl_setopt($ch, CURLOPT_REFERER, $refer );

    }

    curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
    return $ch;
}

/**
 * Limpeza ao terminar de executar
 * Requer $ws
 */
function ws_shutdown(){
	global $ws;

	if (file_exists($ws['cookie'])) {
		unlink($ws['cookie']);
	}
}
register_shutdown_function('ws_shutdown');


function websysRequest($aPost, $sModule = 'login', $sConn = 'http://www.averbeporto.com.br/websys/php/conn.php') {
	global $ws;

	if (!isset($aPost['comp'])) { $aPost['comp'] = $ws['comp']; }
	if (!isset($aPost['path'])) { $aPost['path'] = $ws['path']; } elseif ($aPost['path'] == '') { 		unset($aPost['path']); }
	$aPost['mod'] = $sModule;

	$ch = open_https_url($sConn, '', $ws['cookie']);

	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_POSTFIELDS, $aPost);

	$res = curl_exec($ch);
	print $res . "\n";
	curl_close($ch);

	return $res;
}
?>

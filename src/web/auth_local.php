<?php

class LocalAuthenication extends Authenication
{
    public function authenicate ($credential, $db) {
        if (! isset($credential["user"]) || ! isset ($credential["password"])) {
            return false;
        }
        $passhash = hash ('sha256', $credential["password"]);
        $query = $db->prepare ('select count(*) as cnt from users where name = ? and password = ?');
        $query->execute (array ($credential["user"], $passhash));
        $row = $query->fetch(PDO::FETCH_OBJ);
        return ($row->cnt == 1);
    }
}

?>

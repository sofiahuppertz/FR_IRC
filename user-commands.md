# General commands for FT_IRC

* Change Nickname:

```bash
NICK newNick
```

* Set User Information:

```bash
USER username hostname servername :Real Name
```

* Join Channel:

```bash
JOIN #channel
```


* Leave Channel:

```bash
PART #channel
```

* Private Message:

```bash
PRIVMSG target :Hello there!
```

* Kick User:

```bash
KICK #channel user :Reason for kick
```

* Invite User:

```bash
INVITE user #channel
```

* Change Topic:

```bash
TOPIC #channel :New topic
```

* Pass Command:

```bash
PASS <password>
```

## Mode Commands

* Set Invite-Only Mode

```bash
MODE #channel +i
MODE #channel -i
```


* Restrict Topic Change to Operators:

```bash
MODE #channel +t
MODE #channel -t
```

* Set Channel Key:

```bash
MODE #channel +k password
MODE #channel -k 
```

* Channel Operator Privilege (+o):

```bash
MODE #channel +o nickname
MODE #channel -o nickname
```


* Set User Limit:

```bash
MODE #channel +l 50
MODE #channel -l
```

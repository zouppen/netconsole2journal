# Files for systemd

Drop these files to `/etc/systemd/system` and update paths to your needs. Then just:

```
systemctl daemon-reload
systemctl start netconsole2journal.socket
systemctl enable netconsole2journal.socket
```

Remember to use non-privileged user. Even though you are listening to
UDP port <1024, systemd does that for you and you can run the software
without root privileges and that's how you should do it.

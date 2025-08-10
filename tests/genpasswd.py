#!/usr/bin/env python
import secrets
import string
from rich import console, print
console = console.Console()

class PasswordError(BaseException):
    """Raise this error if generate_strong_password(length) with length < 13"""
    def __init__(self, m=None) -> None:
        if m is None:
            m = "The password must be greater than 13 strings to get strong enough."
        super().__init__(m)

class TooManyStringsError(BaseException):
    """Like PasswordError, but raise error if length > 100"""
    def __init__(self, mn=None) -> None:
        if mn is None:
            mn = "The password is too long for the logs (over 100 strings), and we think that you cannot remember that password."
        super().__init__(mn)

try:
    # Generate the strong password:
    def generate_strong_password(length=20):
        """Output the password with a specified length (13 to 100). Default: length = 20"""
        if length < 13:
            raise PasswordError
        if length > 100:
            raise TooManyStringsError
    
        charset = string.ascii_letters + string.digits + string.punctuation
        password = ''.join(secrets.choice(charset) for _ in range(length))
    
        return password 
    
    # For generating:
    """Generate a password with a specified length. Not for checking output, but it has a warning and password generate style."""
    def generate(length) -> None:
        x = generate_strong_password(length)
        print("[bold blue](+) Strong password: [/bold blue]")
        print(f"[bold white]---->[/bold white]   [bold yellow]{x}[/bold yellow]")
        print("[bold red](!) WARNING! DO NOT SHARE THAT PASSWORD TO ANYONE. THAT THING NEEDS TO BE YOUR SECRET[/bold red]")
        print("[bold orange](-) Note: This is not made for checking the password output! Use genoutput(length) or generate_strong_password(length) instead.")

    # For checking output:
    def genoutput() -> str:
        """Output the password with length = 24"""
        x = generate_strong_password(24)
        return x
except KeyboardInterrupt:
    """Appear if you press Ctrl+C or Ctrl+Break. Exit code: 130"""
    print("""
[bold green]You have canceled the program. Exiting...[/bold green]
          """)
    exit(130)
except PasswordError as passss:
    """Appear if length < 13. Exit code: 1"""
    print(f"[bold]Error: {passss}[/bold]")
    exit(1)
except TooManyStringsError as tmse:
    """Appear if length > 100. Exit code: 1"""
    print(f"[bold]Caution: {tmse}[/bold]")
    exit(1)
except Exception as e:
    """Apper when expected an unknown error. Exit code: 1"""
    print("Error:", f"[bold]{e}[/bold]")
    exit(1)

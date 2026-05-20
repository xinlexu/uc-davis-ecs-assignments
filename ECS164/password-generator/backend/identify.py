from pathlib import Path
from secrets import SystemRandom
import sqlite3

from flask import Flask, jsonify, request
from flask_cors import CORS, cross_origin


DB_PATH = Path(__file__).with_name("identifier.sqlite")
RANDOM = SystemRandom()

app = Flask(__name__)
CORS(app)


def get_db_connection():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn


def init_db():
    conn = get_db_connection()
    conn.execute(
        """
        CREATE TABLE IF NOT EXISTS password_history (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT NOT NULL,
            password TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
        """
    )
    conn.commit()
    conn.close()


def get_password_history(name):
    conn = get_db_connection()
    passwords = conn.execute(
        "SELECT password FROM password_history WHERE name = ? ORDER BY id DESC LIMIT 10",
        (name,),
    ).fetchall()
    conn.close()
    return [password["password"] for password in passwords]


def add_password_history(name, password):
    conn = get_db_connection()
    conn.execute(
        "INSERT INTO password_history (name, password) VALUES (?, ?)",
        (name, password),
    )
    conn.commit()
    conn.close()


def as_bool(value):
    return value is True or str(value).lower() in {"1", "true", "yes", "on"}


def as_int(value, default):
    try:
        return int(value)
    except (TypeError, ValueError):
        return default


def generate_password(params):
    lowercase_letters = "abcdefghijklmnopqrstuvwxyz"
    uppercase_letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    digits = "0123456789"
    symbols = "`!@#$%^&*()_-+=|?/"

    initial = [
        str(params.get(f"q{i}", ""))[:2].capitalize()
        for i in range(1, 6)
        if params.get(f"q{i}")
    ]

    if len(initial) > 2 and initial[1].isdigit():
        base = initial[0] + initial[1] + initial[2]
    else:
        base = "".join(initial)

    include_uppercase = as_bool(params.get("upper"))
    include_lowercase = as_bool(params.get("lower"))
    include_digits = as_bool(params.get("number"))
    include_symbols = as_bool(params.get("symbol"))

    required_chars = []
    if include_uppercase:
        required_chars.append(RANDOM.choice(uppercase_letters))
    if include_lowercase:
        required_chars.append(RANDOM.choice(lowercase_letters))
    if include_digits:
        required_chars.append(RANDOM.choice(digits))
    if include_symbols:
        required_chars.append(RANDOM.choice(symbols))

    password_length = max(
        8,
        as_int(params.get("passwordLen"), 8),
        len(base) + len(required_chars),
    )

    char_pool = lowercase_letters + uppercase_letters
    if include_digits:
        char_pool += digits
    if include_symbols:
        char_pool += symbols

    remaining = password_length - len(base) - len(required_chars)
    extra_characters = required_chars + [
        RANDOM.choice(char_pool) for _ in range(remaining)
    ]
    RANDOM.shuffle(extra_characters)
    return base + "".join(extra_characters)


@app.route("/gener1", methods=["POST"])
def handle_generate_password1():
    if not request.is_json:
        return jsonify({"error": "Request must be JSON"}), 400

    params = request.json
    name = params.get("name")
    return jsonify({"message": "Processed successfully", "name": name})


@app.route("/gener", methods=["POST"])
@cross_origin()
def handle_generate_password():
    params = request.get_json(silent=True) or {}
    name = str(params.get("name", "")).strip()
    if not name:
        return jsonify({"error": "name is required"}), 400

    password_history = get_password_history(name)
    final_password = generate_password(params)
    add_password_history(name, final_password)

    response = {"password": final_password}
    if password_history:
        response["history"] = password_history

    return jsonify(response)


if __name__ == "__main__":
    init_db()
    app.run(debug=True)

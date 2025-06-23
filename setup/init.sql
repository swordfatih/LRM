DO
$$
BEGIN
   IF NOT EXISTS (
      SELECT FROM pg_catalog.pg_roles WHERE rolname = 'mcp_user'
   ) THEN
      CREATE USER mcp_user WITH PASSWORD 'secure_password';
      GRANT CONNECT ON DATABASE lrm TO mcp_user;
      GRANT USAGE ON SCHEMA public TO mcp_user;
      GRANT SELECT, INSERT, UPDATE, DELETE ON ALL TABLES IN SCHEMA public TO mcp_user;
      ALTER DEFAULT PRIVILEGES IN SCHEMA public GRANT SELECT, INSERT, UPDATE, DELETE ON TABLES TO mcp_user;
   END IF;
END
$$;